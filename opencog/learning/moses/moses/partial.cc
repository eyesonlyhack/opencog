/** partial.cc ---
 *
 * Copyright (C) 2012 Poulin Holdings
 *
 * Author: Linas Vepstas <linasvepstas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "partial.h"
#include "scoring.h"

namespace opencog { namespace moses {

using namespace std;

typedef combo_tree::sibling_iterator sib_it;
typedef combo_tree::iterator pre_it;

partial_solver::partial_solver(const vector<CTable> &ctables,
                               int as, float noise,
                               const type_tree& table_tt,
                               const vector<combo_tree>& exemplars,
                               const rule& reduct,
                               const optim_parameters& opt_params,
                               const metapop_parameters& meta_params,
                               const moses_parameters& moses_params,
                               const metapop_printer& mmr_pa)

    :_ctables(ctables), _alf_sz(as), _noise(noise),
     _table_type_signature(table_tt),
     _exemplars(exemplars), _reduct(reduct),
     _opt_params(opt_params), _meta_params(meta_params),
     _moses_params(moses_params), _mmr_pa(mmr_pa),
     _fail_recurse_count(0), _best_fail_ratio(1.0e30),
     _best_fail_tree(combo_tree()),
     _best_fail_pred(_best_fail_tree.end())
{
}

partial_solver::~partial_solver()
{
    delete _bscore;
}

/// Implements the "leave well-enough alone" algorithm.
void partial_solver::solve()
{
    _done = false;

    unsigned tab_sz = 0;
    boost::ptr_vector<BScore> bscores;
    foreach(const CTable& ctable, _ctables) {
        // FYI, no mem-leak, as ptr_vector seems to call delete.
        bscores.push_back(new BScore(ctable, _alf_sz, _noise));

        tab_sz += ctable.uncompressed_size();
    }
    _bscore = new multibscore_based_bscore<BScore>(bscores);

#define FRACTION 0.5
    _bad_score = - floor(FRACTION * score_t(tab_sz));

    for(int i=0; i<100; i++) {
        _opt_params.terminate_if_gte = _bad_score;
        _moses_params.max_score = _bad_score;

cout <<"duuude start loop ===================== rec="<<_fail_recurse_count <<" ======= "<<i<<" ask="<<_bad_score<<endl;
        metapop_moses_results(_exemplars, _table_type_signature,
                              _reduct, _reduct, *_bscore,
                              _opt_params, _meta_params, _moses_params,
                              *this);
        if (_done) break;
    }
cout<<"duuu YYYYYYYYYYYYYYYYYYYYYYYYYYYYY DONE !"<<endl;
}

/// Evaluate each of the candidate solutions, see if any of the leading
/// predicates are "good enough".  If we find one that is, then trim
/// the scoring tables, and return (so as to run moses on the smaller
/// problem).
void partial_solver::candidates(const metapop_candidates& cands)
{
cout<<"duuude got cands sz="<<cands.size()<<endl;
    foreach(auto &item, cands) {
        const combo_tree& cand = item.first;
        if (candidate(cand)) return;
    }

    if (_best_fail_ratio < 0.2) {
        if (recurse()) return;
    }

    // If we are here, then none of the candidates were any good.
    // Tighten up the score, and try again.
    // XXX this is wrong, should be fraction of the max score.
    _bad_score = ceil(0.8 *_bad_score);

    foreach(BScore& bs, _bscore->bscores)
        bs.punish += 1.0;

cout <<"duuude nothing good, try aaing with score="<<_bad_score<<endl;
}

/// Remove all rows from the table that satisfy the predicate.
void partial_solver::trim_table(std::vector<CTable>& tabs,
                                const combo_tree::iterator predicate,
                                unsigned& deleted,   // return value
                                unsigned& total)    // return value

{
    foreach(CTable& ctable, tabs) {
        for (CTable::iterator cit = ctable.begin(); cit != ctable.end(); ) {
            const vertex_seq& vs = cit->first;
            const CTable::counter_t& c = cit->second;

            unsigned tc = c.total_count();
            total += tc;

            vertex pr = eval_throws_binding(vs, predicate);
            if (pr == id::logical_true) {
                deleted += tc;
                ctable.erase(cit++);
            }
            else cit++;
        }
    }
}

/// Evaluate a single candidate
bool partial_solver::candidate (const combo_tree& cand)
{
std::cout<<"duude in the candy="<<cand<<std::endl;

    // Are we done yet?
    behavioral_score bs = _bscore->operator()(cand);
    score_t total_score = 0.0;
    foreach(const score_t& sc, bs)
        total_score += sc;

    // XXX replace  by the correct compare, i.e. the orig gte.
    if (-0.5 <= total_score) {
std::cout<<"duuude DOOOOOOOOOONE! ="<<total_score<<"\n"<<std::endl;
        _done = true;
        return true;
    }
std::cout<<"duuude candy score total="<<total_score<<std::endl;

    // We don't want constants; what else we got?
    pre_it it = cand.begin();
    if (is_enum_type(*it)) {
std::cout<<"duuude got a const\n"<<std::endl;
        return false;
    }

    OC_ASSERT(*it == id::cond, "Error: unexpected candidate!");

int total;
    // Yank out the first effective predicate, and evaluate it's accuracy
    // A predicate is effective if it makes at least one correct
    // identification.  Otherwise, it may as well be "always false"
    // and is ineffective at identifyng anything.
    sib_it sib = it.begin();
    sib_it predicate = sib;
    unsigned fail_count = 0;
    unsigned good_count = 0;
    while(1) {

        if (is_enum_type(*it)) {
            // If we are here, all previous predicates were ineffective.
std::cout<<"duuude got an ineffective const\n"<<std::endl;
            return false;
        }

        predicate = sib;
        sib++;
        vertex consequent = *sib;
        sib++;

total = 0;
        // Count how many items the first predicate mis-identifies.
        fail_count = 0;
        good_count = 0;
        foreach(CTable& ctable, _ctables) {
            for (CTable::iterator cit = ctable.begin(); cit != ctable.end(); cit++) {
                const vertex_seq& vs = cit->first;
                const CTable::counter_t& c = cit->second;

total += c.total_count();
                vertex pr = eval_throws_binding(vs, predicate);
                if (pr == id::logical_true) {
                    if (c.total_count() != c.get(consequent))
                        fail_count++;
                    else
                        good_count++;
                }
            }
        }
std::cout<<"duuude tot="<<total<<" fail ="<<fail_count <<" good="<<good_count<<std::endl;

        // XXX Ineffective predicates may be due to enums that have been
        // completely accounted for ... not sure what to do about that...
        if (0 < good_count)
            break;
    }

    // If the fail count isn't zero, then punish more strongly, and try again.
    if (fail_count) {
        double fail_ratio = double(fail_count) / double(good_count);
        if (fail_ratio < _best_fail_ratio) {
            _best_fail_ratio = fail_ratio;
            _best_fail_tree = cand;
            _best_fail_pred = predicate;
        }

cout<<"duuude non zero fail count\n"<<endl;
        return false;
    }

    // If we are here, the first predicate is correctly identifying
    // all cases; we can save it, and remove all table elements that
    // it correctly identified.
    unsigned total_rows = 0;
    unsigned deleted = 0;
    trim_table(_ctables, predicate, deleted, total_rows);

    // XXX replace 0.5 by parameter.
    // XXX should be, ummm, less than the number of a single type in the table,
    // else a constant beats this score...
    _bad_score = -0.45 * (score_t(total_rows) - score_t(deleted));
cout<<"duude deleted="<<deleted <<" out of total="<<total_rows<< " gonna ask for score of="<<_bad_score<<endl;

    // Redo the scoring tables, as they cache the score tables (why?)
    boost::ptr_vector<BScore> bscores;
    foreach(const CTable& ctable, _ctables) {
        // FYI, no mem-leak, as ptr_vector seems to call delete.
        // XXX !?!?! relly?  as we've got a lifetime problem here.
        bscores.push_back(new BScore(ctable, _alf_sz, _noise));
    }
    delete _bscore;
    _bscore = new multibscore_based_bscore<BScore>(bscores);

cout<<endl;
    return true;
}

/// Recurse, and try to weed out a few more cases.
bool partial_solver::recurse()
{
    if (1 < _fail_recurse_count)
        return false;

    std::vector<CTable> tabs;

    // Make a copy of the tables
    foreach(CTable& ctable, _ctables) {
        tabs.push_back(CTable(ctable));
    }

    // Remove unwanted rows from the copy
    unsigned total_rows = 0;
    unsigned deleted = 0;
    trim_table(tabs, _best_fail_pred, deleted, total_rows);
cout<<"duude before recusion, deleted="<< deleted<<" out of="<<total_rows<<endl;

    // And lets try to find a predicate that does the trick.
    vector<combo_tree> exs;
    partial_solver ps(tabs, 
                      _alf_sz, _noise, _table_type_signature,
                      exs, _reduct, _opt_params, _meta_params,
                      _moses_params, _mmr_pa);

cout<<"duuu vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"<<endl;
    ps._fail_recurse_count++;
    ps.solve();
cout<<"duu ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;

    return false;
}

};};