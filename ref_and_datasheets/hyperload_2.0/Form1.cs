using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace MegaLoad_.NET
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MegaLoadForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox FlashFileName;
		private System.Windows.Forms.Button FlashOpen;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.TextBox EEpromFileName;
		private System.Windows.Forms.Button EEpromOpen;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.CheckBox BLB12;
		private System.Windows.Forms.CheckBox BLB11;
		private System.Windows.Forms.CheckBox BLB02;
		private System.Windows.Forms.CheckBox BLB01;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.ComboBox comboBox1;
		private System.Windows.Forms.ComboBox CommSpeed;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.CheckBox DSR;
		private System.Windows.Forms.CheckBox RTS;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label Device;
		private System.Windows.Forms.Label PageSize;
		private System.Windows.Forms.Label BootSize;
		private System.Windows.Forms.Label FlashSize;
		private System.Windows.Forms.Label EEpromSize;
		private System.Windows.Forms.GroupBox groupBox7;
		private System.Windows.Forms.TextBox StatusText;
		private System.Windows.Forms.GroupBox groupBox8;
		private System.Windows.Forms.GroupBox groupBox9;
		private System.Windows.Forms.CheckBox EEpromProg;
		private System.Windows.Forms.CheckBox BootReset;
		private System.Windows.Forms.ListBox MessageList;
		private System.Windows.Forms.ProgressBar ProgressBar;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MegaLoadForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

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
				if (components != null) 
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.FlashFileName = new System.Windows.Forms.TextBox();
			this.FlashOpen = new System.Windows.Forms.Button();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.EEpromFileName = new System.Windows.Forms.TextBox();
			this.EEpromOpen = new System.Windows.Forms.Button();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.BLB12 = new System.Windows.Forms.CheckBox();
			this.BLB11 = new System.Windows.Forms.CheckBox();
			this.BLB02 = new System.Windows.Forms.CheckBox();
			this.BLB01 = new System.Windows.Forms.CheckBox();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.groupBox5 = new System.Windows.Forms.GroupBox();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.comboBox1 = new System.Windows.Forms.ComboBox();
			this.CommSpeed = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.DSR = new System.Windows.Forms.CheckBox();
			this.RTS = new System.Windows.Forms.CheckBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.Device = new System.Windows.Forms.Label();
			this.PageSize = new System.Windows.Forms.Label();
			this.BootSize = new System.Windows.Forms.Label();
			this.FlashSize = new System.Windows.Forms.Label();
			this.EEpromSize = new System.Windows.Forms.Label();
			this.groupBox7 = new System.Windows.Forms.GroupBox();
			this.StatusText = new System.Windows.Forms.TextBox();
			this.groupBox8 = new System.Windows.Forms.GroupBox();
			this.groupBox9 = new System.Windows.Forms.GroupBox();
			this.EEpromProg = new System.Windows.Forms.CheckBox();
			this.BootReset = new System.Windows.Forms.CheckBox();
			this.MessageList = new System.Windows.Forms.ListBox();
			this.ProgressBar = new System.Windows.Forms.ProgressBar();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox5.SuspendLayout();
			this.groupBox6.SuspendLayout();
			this.groupBox7.SuspendLayout();
			this.groupBox8.SuspendLayout();
			this.groupBox9.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.FlashOpen);
			this.groupBox1.Controls.Add(this.FlashFileName);
			this.groupBox1.Location = new System.Drawing.Point(8, 16);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(504, 48);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "File to be program in the Flash";
			// 
			// FlashFileName
			// 
			this.FlashFileName.Location = new System.Drawing.Point(8, 16);
			this.FlashFileName.Name = "FlashFileName";
			this.FlashFileName.Size = new System.Drawing.Size(400, 20);
			this.FlashFileName.TabIndex = 0;
			this.FlashFileName.Text = "";
			// 
			// FlashOpen
			// 
			this.FlashOpen.Location = new System.Drawing.Point(424, 16);
			this.FlashOpen.Name = "FlashOpen";
			this.FlashOpen.Size = new System.Drawing.Size(64, 24);
			this.FlashOpen.TabIndex = 1;
			this.FlashOpen.Text = "Open";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.EEpromOpen);
			this.groupBox2.Controls.Add(this.EEpromFileName);
			this.groupBox2.Location = new System.Drawing.Point(8, 80);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(504, 48);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "File to be program in the EEprom";
			// 
			// EEpromFileName
			// 
			this.EEpromFileName.Location = new System.Drawing.Point(8, 16);
			this.EEpromFileName.Name = "EEpromFileName";
			this.EEpromFileName.Size = new System.Drawing.Size(400, 20);
			this.EEpromFileName.TabIndex = 0;
			this.EEpromFileName.Text = "";
			// 
			// EEpromOpen
			// 
			this.EEpromOpen.Location = new System.Drawing.Point(424, 16);
			this.EEpromOpen.Name = "EEpromOpen";
			this.EEpromOpen.Size = new System.Drawing.Size(64, 24);
			this.EEpromOpen.TabIndex = 1;
			this.EEpromOpen.Text = "Open";
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.label1);
			this.groupBox3.Controls.Add(this.BLB01);
			this.groupBox3.Controls.Add(this.BLB12);
			this.groupBox3.Controls.Add(this.BLB11);
			this.groupBox3.Controls.Add(this.BLB02);
			this.groupBox3.Location = new System.Drawing.Point(8, 144);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(504, 40);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "BootLoader Lock Bits to be program";
			// 
			// BLB12
			// 
			this.BLB12.Location = new System.Drawing.Point(16, 16);
			this.BLB12.Name = "BLB12";
			this.BLB12.Size = new System.Drawing.Size(64, 16);
			this.BLB12.TabIndex = 3;
			this.BLB12.Text = "BLB12";
			// 
			// BLB11
			// 
			this.BLB11.Location = new System.Drawing.Point(80, 16);
			this.BLB11.Name = "BLB11";
			this.BLB11.Size = new System.Drawing.Size(64, 16);
			this.BLB11.TabIndex = 4;
			this.BLB11.Text = "BLB11";
			// 
			// BLB02
			// 
			this.BLB02.Location = new System.Drawing.Point(144, 16);
			this.BLB02.Name = "BLB02";
			this.BLB02.Size = new System.Drawing.Size(64, 16);
			this.BLB02.TabIndex = 5;
			this.BLB02.Text = "BLB02";
			// 
			// BLB01
			// 
			this.BLB01.Location = new System.Drawing.Point(208, 16);
			this.BLB01.Name = "BLB01";
			this.BLB01.Size = new System.Drawing.Size(64, 16);
			this.BLB01.TabIndex = 6;
			this.BLB01.Text = "BLB01";
			// 
			// label1
			// 
			this.label1.ForeColor = System.Drawing.SystemColors.GrayText;
			this.label1.Location = new System.Drawing.Point(296, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(192, 16);
			this.label1.TabIndex = 7;
			this.label1.Text = "Check means programmed (bit = 0)";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.RTS);
			this.groupBox4.Controls.Add(this.DSR);
			this.groupBox4.Controls.Add(this.label3);
			this.groupBox4.Controls.Add(this.label2);
			this.groupBox4.Controls.Add(this.CommSpeed);
			this.groupBox4.Controls.Add(this.comboBox1);
			this.groupBox4.Location = new System.Drawing.Point(8, 200);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(168, 112);
			this.groupBox4.TabIndex = 3;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Comm Setup";
			// 
			// groupBox5
			// 
			this.groupBox5.Controls.Add(this.Device);
			this.groupBox5.Controls.Add(this.label8);
			this.groupBox5.Controls.Add(this.label7);
			this.groupBox5.Controls.Add(this.label6);
			this.groupBox5.Controls.Add(this.label5);
			this.groupBox5.Controls.Add(this.label4);
			this.groupBox5.Controls.Add(this.PageSize);
			this.groupBox5.Controls.Add(this.BootSize);
			this.groupBox5.Controls.Add(this.FlashSize);
			this.groupBox5.Controls.Add(this.EEpromSize);
			this.groupBox5.Location = new System.Drawing.Point(184, 200);
			this.groupBox5.Name = "groupBox5";
			this.groupBox5.Size = new System.Drawing.Size(152, 184);
			this.groupBox5.TabIndex = 4;
			this.groupBox5.TabStop = false;
			this.groupBox5.Text = "Target";
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.MessageList);
			this.groupBox6.Location = new System.Drawing.Point(344, 200);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(160, 296);
			this.groupBox6.TabIndex = 5;
			this.groupBox6.TabStop = false;
			this.groupBox6.Text = "Messages";
			// 
			// comboBox1
			// 
			this.comboBox1.Items.AddRange(new object[] {
														   "Com1",
														   "Com2",
														   "Com3",
														   "Com4",
														   "Com5",
														   "Com6",
														   "Com7",
														   "Com8",
														   "Com9",
														   "Com10",
														   "Com12",
														   "Com13",
														   "Com14",
														   "Com15",
														   "Com16",
														   "Com17",
														   "Com18",
														   "Com19"});
			this.comboBox1.Location = new System.Drawing.Point(8, 40);
			this.comboBox1.Name = "comboBox1";
			this.comboBox1.Size = new System.Drawing.Size(72, 21);
			this.comboBox1.TabIndex = 0;
			this.comboBox1.Text = "ComPort";
			// 
			// CommSpeed
			// 
			this.CommSpeed.Items.AddRange(new object[] {
														   "9600bps",
														   "19200bps",
														   "38400bps",
														   "57600bps",
														   "115200bps"});
			this.CommSpeed.Location = new System.Drawing.Point(88, 40);
			this.CommSpeed.Name = "CommSpeed";
			this.CommSpeed.Size = new System.Drawing.Size(72, 21);
			this.CommSpeed.TabIndex = 1;
			this.CommSpeed.Text = "Speed";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 24);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(64, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "CommPort";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(88, 24);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(64, 16);
			this.label3.TabIndex = 3;
			this.label3.Text = "Speed";
			// 
			// DSR
			// 
			this.DSR.Location = new System.Drawing.Point(8, 80);
			this.DSR.Name = "DSR";
			this.DSR.Size = new System.Drawing.Size(72, 16);
			this.DSR.TabIndex = 4;
			this.DSR.Text = "DSR";
			// 
			// RTS
			// 
			this.RTS.Location = new System.Drawing.Point(88, 80);
			this.RTS.Name = "RTS";
			this.RTS.Size = new System.Drawing.Size(72, 16);
			this.RTS.TabIndex = 5;
			this.RTS.Text = "RTS";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 24);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(72, 16);
			this.label4.TabIndex = 0;
			this.label4.Text = "Device:";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(8, 56);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(72, 16);
			this.label5.TabIndex = 1;
			this.label5.Text = "PageSize:";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(8, 88);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(72, 16);
			this.label6.TabIndex = 2;
			this.label6.Text = "BootSize:";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 120);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(72, 16);
			this.label7.TabIndex = 3;
			this.label7.Text = "Flash Size:";
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(8, 152);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(72, 16);
			this.label8.TabIndex = 4;
			this.label8.Text = "EEpromSize:";
			// 
			// Device
			// 
			this.Device.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.Device.ForeColor = System.Drawing.SystemColors.ActiveCaption;
			this.Device.Location = new System.Drawing.Point(80, 24);
			this.Device.Name = "Device";
			this.Device.Size = new System.Drawing.Size(64, 16);
			this.Device.TabIndex = 5;
			this.Device.Text = "xxxx";
			// 
			// PageSize
			// 
			this.PageSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.PageSize.ForeColor = System.Drawing.SystemColors.ActiveCaption;
			this.PageSize.Location = new System.Drawing.Point(80, 56);
			this.PageSize.Name = "PageSize";
			this.PageSize.Size = new System.Drawing.Size(64, 16);
			this.PageSize.TabIndex = 6;
			this.PageSize.Text = "xxxx";
			// 
			// BootSize
			// 
			this.BootSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.BootSize.ForeColor = System.Drawing.SystemColors.ActiveCaption;
			this.BootSize.Location = new System.Drawing.Point(80, 88);
			this.BootSize.Name = "BootSize";
			this.BootSize.Size = new System.Drawing.Size(64, 16);
			this.BootSize.TabIndex = 7;
			this.BootSize.Text = "xxxx";
			// 
			// FlashSize
			// 
			this.FlashSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.FlashSize.ForeColor = System.Drawing.SystemColors.ActiveCaption;
			this.FlashSize.Location = new System.Drawing.Point(80, 120);
			this.FlashSize.Name = "FlashSize";
			this.FlashSize.Size = new System.Drawing.Size(64, 16);
			this.FlashSize.TabIndex = 8;
			this.FlashSize.Text = "xxxx";
			// 
			// EEpromSize
			// 
			this.EEpromSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.EEpromSize.ForeColor = System.Drawing.SystemColors.ActiveCaption;
			this.EEpromSize.Location = new System.Drawing.Point(80, 152);
			this.EEpromSize.Name = "EEpromSize";
			this.EEpromSize.Size = new System.Drawing.Size(64, 16);
			this.EEpromSize.TabIndex = 9;
			this.EEpromSize.Text = "xxxx";
			// 
			// groupBox7
			// 
			this.groupBox7.Controls.Add(this.StatusText);
			this.groupBox7.Location = new System.Drawing.Point(8, 392);
			this.groupBox7.Name = "groupBox7";
			this.groupBox7.Size = new System.Drawing.Size(328, 48);
			this.groupBox7.TabIndex = 6;
			this.groupBox7.TabStop = false;
			this.groupBox7.Text = "Status";
			// 
			// StatusText
			// 
			this.StatusText.Location = new System.Drawing.Point(8, 16);
			this.StatusText.Name = "StatusText";
			this.StatusText.Size = new System.Drawing.Size(312, 20);
			this.StatusText.TabIndex = 0;
			this.StatusText.Text = "";
			// 
			// groupBox8
			// 
			this.groupBox8.Controls.Add(this.ProgressBar);
			this.groupBox8.Location = new System.Drawing.Point(8, 448);
			this.groupBox8.Name = "groupBox8";
			this.groupBox8.Size = new System.Drawing.Size(328, 48);
			this.groupBox8.TabIndex = 7;
			this.groupBox8.TabStop = false;
			this.groupBox8.Text = "Progress";
			// 
			// groupBox9
			// 
			this.groupBox9.Controls.Add(this.BootReset);
			this.groupBox9.Controls.Add(this.EEpromProg);
			this.groupBox9.Location = new System.Drawing.Point(8, 320);
			this.groupBox9.Name = "groupBox9";
			this.groupBox9.Size = new System.Drawing.Size(168, 64);
			this.groupBox9.TabIndex = 8;
			this.groupBox9.TabStop = false;
			this.groupBox9.Text = "Options";
			// 
			// EEpromProg
			// 
			this.EEpromProg.Location = new System.Drawing.Point(8, 16);
			this.EEpromProg.Name = "EEpromProg";
			this.EEpromProg.Size = new System.Drawing.Size(144, 16);
			this.EEpromProg.TabIndex = 0;
			this.EEpromProg.Text = "EEprom Programing";
			// 
			// BootReset
			// 
			this.BootReset.Location = new System.Drawing.Point(8, 40);
			this.BootReset.Name = "BootReset";
			this.BootReset.Size = new System.Drawing.Size(152, 16);
			this.BootReset.TabIndex = 1;
			this.BootReset.Text = "BootLoad on Reset";
			// 
			// MessageList
			// 
			this.MessageList.Location = new System.Drawing.Point(8, 16);
			this.MessageList.Name = "MessageList";
			this.MessageList.Size = new System.Drawing.Size(144, 264);
			this.MessageList.TabIndex = 0;
			// 
			// ProgressBar
			// 
			this.ProgressBar.Location = new System.Drawing.Point(8, 16);
			this.ProgressBar.Name = "ProgressBar";
			this.ProgressBar.Size = new System.Drawing.Size(312, 24);
			this.ProgressBar.TabIndex = 0;
			// 
			// MegaLoadForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(520, 590);
			this.Controls.Add(this.groupBox9);
			this.Controls.Add(this.groupBox8);
			this.Controls.Add(this.groupBox7);
			this.Controls.Add(this.groupBox6);
			this.Controls.Add(this.groupBox5);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Name = "MegaLoadForm";
			this.Text = "MegaLoad .NET V:4.0b1";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox5.ResumeLayout(false);
			this.groupBox6.ResumeLayout(false);
			this.groupBox7.ResumeLayout(false);
			this.groupBox8.ResumeLayout(false);
			this.groupBox9.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
		
		}
	}
}
