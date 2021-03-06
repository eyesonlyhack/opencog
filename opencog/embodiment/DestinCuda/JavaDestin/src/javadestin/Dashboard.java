/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * Dashboard.java
 *
 * Created on Nov 8, 2011, 12:58:41 PM
 */
/**
 *
 * @author ted
 */
package javadestin;
public class Dashboard extends javax.swing.JFrame {
	IExperiment exp = new VideoExperiment( new NetworkFactory(false));
	
	/** Creates new form Dashboard */
	public Dashboard() {
		initComponents();
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Netbeans IDE Form Editor. 
	 */
	@SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        chkPosTraining = new javax.swing.JCheckBox();
        chkPSSATraining = new javax.swing.JCheckBox();
        btStart = new javax.swing.JButton();
        btStop = new javax.swing.JButton();
        btReset = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("DeSTIN Dashboard");

        chkPosTraining.setSelected(true);
        chkPosTraining.setText("P(o|s) Training");
        chkPosTraining.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkPosTrainingActionPerformed(evt);
            }
        });

        chkPSSATraining.setSelected(true);
        chkPSSATraining.setText("P(s'|s,a) Training");
        chkPSSATraining.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkPSSATrainingActionPerformed(evt);
            }
        });

        btStart.setText("Start");
        btStart.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btStartActionPerformed(evt);
            }
        });

        btStop.setText("Stop");
        btStop.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btStopActionPerformed(evt);
            }
        });

        btReset.setText("Reset");
        btReset.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btResetActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(chkPosTraining)
                        .addGap(18, 18, 18)
                        .addComponent(chkPSSATraining))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(btStart)
                        .addGap(18, 18, 18)
                        .addComponent(btStop)
                        .addGap(18, 18, 18)
                        .addComponent(btReset)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(chkPosTraining)
                    .addComponent(chkPSSATraining))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 18, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btStart)
                    .addComponent(btStop)
                    .addComponent(btReset))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

	private void btStartActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btStartActionPerformed
		exp.start();
	}//GEN-LAST:event_btStartActionPerformed

	private void btStopActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btStopActionPerformed
		exp.stop();
	}//GEN-LAST:event_btStopActionPerformed

	private void chkPosTrainingActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkPosTrainingActionPerformed
		exp.setPOSTraining(chkPosTraining.isSelected());
	}//GEN-LAST:event_chkPosTrainingActionPerformed

	private void chkPSSATrainingActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkPSSATrainingActionPerformed
		exp.setPSSATraining(chkPSSATraining.isSelected());
	}//GEN-LAST:event_chkPSSATrainingActionPerformed

	private void btResetActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btResetActionPerformed
		exp.reset();
	}//GEN-LAST:event_btResetActionPerformed

	/**
	 * @param args the command line arguments
	 */
	public static void main(String args[]) {
		/* Set the Nimbus look and feel */
		//<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
		 * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
		 */
		try {
			for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
				if ("Nimbus".equals(info.getName())) {
					javax.swing.UIManager.setLookAndFeel(info.getClassName());
					break;
				}
			}
		} catch (ClassNotFoundException ex) {
			java.util.logging.Logger.getLogger(Dashboard.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
		} catch (InstantiationException ex) {
			java.util.logging.Logger.getLogger(Dashboard.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
		} catch (IllegalAccessException ex) {
			java.util.logging.Logger.getLogger(Dashboard.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
		} catch (javax.swing.UnsupportedLookAndFeelException ex) {
			java.util.logging.Logger.getLogger(Dashboard.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
		}
		//</editor-fold>

		/* Create and display the form */
		java.awt.EventQueue.invokeLater(new Runnable() {

			public void run() {
				new Dashboard().setVisible(true);
			}
		});
	}
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btReset;
    private javax.swing.JButton btStart;
    private javax.swing.JButton btStop;
    private javax.swing.JCheckBox chkPSSATraining;
    private javax.swing.JCheckBox chkPosTraining;
    // End of variables declaration//GEN-END:variables
}
