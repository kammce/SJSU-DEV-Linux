using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace MegaLoad_.NET
{
	/// <summary>
	/// Summary description for AboutForm.
	/// </summary>
	public class AboutForm : System.Windows.Forms.Form
    {
		private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ListBox listBox1;

		private HyperLoad MegaLoad;
        //private System.IO.Ports.SerialPort Comm = null;
        private System.Windows.Forms.GroupBox groupBox1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

        public AboutForm(HyperLoad FormArg, System.IO.Ports.SerialPort CommArg, bool Reg)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			MegaLoad = FormArg;
			// Comm = CommArg;
            // Comm.Close();// Comm.Active = false;

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutForm));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(64, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(248, 32);
            this.label1.TabIndex = 1;
            this.label1.Text = "HyperLoad 1.0";
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(127, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 24);
            this.label2.TabIndex = 2;
            this.label2.Text = "SJValley";
            // 
            // listBox1
            // 
            this.listBox1.Items.AddRange(new object[] {
            "This program is completely free to use!",
            "If you like it, please distribute the installer package.",
            "",
            "HyperLoad can detect the target at various",
            "different baud rate settings and lock in at",
            "different speeds selected by you during programming.",
            "",
            "Hyperload requires bootloader Software that can be",
            "requested from SJValley for free!",
            "Usually the turn around time is SAME DAY!",
            "",
            "Please note that to use \"RESET\" button, your target",
            "needs to detect \"$RESET\" string and then perform",
            "a jump or watchdog reset after 25ms.",
            "",
            "For questions and support, please contact:",
            "support@sjvalley.com"});
            this.listBox1.Location = new System.Drawing.Point(14, 84);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(304, 108);
            this.listBox1.TabIndex = 5;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.listBox1);
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(320, 210);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            // 
            // AboutForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(338, 225);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "About";
            this.Deactivate += new System.EventHandler(this.AboutForm_Deactivate);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

		}
		#endregion

		private void linkLabel2_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start("www.microsyl.com");

		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start("mailto:sbissonnette@microsyl.com");

		}


		private void AboutForm_Deactivate(object sender, System.EventArgs e)
		{
			this.Close();
		}

        private void linkLabel3_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("www.imagecraft.com/software/");
        }
	}
}
