using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using Microsoft.Win32;
using System.Threading;

namespace MegaLoad_.NET
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class HyperLoad : System.Windows.Forms.Form
	{
        private int INITIAL_BAUD_RATE = 38400;
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
		private System.Windows.Forms.GroupBox groupBox_comport;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.ComboBox CommSpeed;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
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
        private System.Windows.Forms.GroupBox groupBox8;
		private System.Windows.Forms.ListBox MessageList;
        private System.Windows.Forms.ProgressBar ProgressBar;
		private System.Windows.Forms.OpenFileDialog OpenFileDialog;
		private System.Windows.Forms.ComboBox PortSelect;

        private int portUpdateTimer = 0;
		private byte[] Flash;
		private int FlashMin;
		private int FlashMax;

		private byte[] EEprom;
		private bool[] EEpromUse;
		private int EEpromMin;
		private int EEpromMax;
		private int BytePtr;

		private int PageSizeInt;
		private int FlashSizeInt;
		private int BootSizeInt;
		private int EEpromSizeInt;
		private int PagePtr;
		private int Retry;
		//private char MemType;
        private int FLASH_FILE_USAGE;
        private int serialDataErrorCountTimer = 0;

        private int lockBitProgrammingSupportedByCPU=0;
        private int eepromProgrammingSupportedByCPU=0;

        private bool Reg = false; 
		private string FlashFileNameHex = "";
        private string EEpromFileNameHex = "";
		private System.Windows.Forms.CheckBox DTR;
        private System.Windows.Forms.Button SendReset;

		private AboutForm About;
        private System.Windows.Forms.Button PortOpenButton;
		private System.Windows.Forms.Button AboutButton;
        private System.Windows.Forms.ToolTip ToolTip;
        private System.IO.Ports.SerialPort serialPort;
        private Button clearButtonFLASH;
        private Button clearButtonEEPROM;
        private CheckBox twoStopBits;
        private Button messageClearButton;
        private Button PortCloseButton;
        private CheckBox lockBootLoader;
        private CheckBox lockProgram;
        private Button lockOptions;
        private CheckBox closePortAfterProgramming;
        private CheckBox disableProgrammingCheckBox;
        private System.Windows.Forms.Timer timer1;
        private Button errorInfoButton;
        private BackgroundWorker cpuProgrammingThread;
        private Label label1;
        private TextBox textBox_crystal;
        private Button button1;
        private GroupBox groupBox9;
        private Button button_background;
		private System.ComponentModel.IContainer components;

		public HyperLoad()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

            // Add available ports to the port selection drop-down menu
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            foreach (string s in ports)
            {
                this.PortSelect.Items.Add(s);
            }

            // Run the thread that will help program the CPU and monitor serial data
            cpuProgrammingThread.RunWorkerAsync();
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HyperLoad));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.clearButtonFLASH = new System.Windows.Forms.Button();
            this.FlashOpen = new System.Windows.Forms.Button();
            this.FlashFileName = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.clearButtonEEPROM = new System.Windows.Forms.Button();
            this.EEpromOpen = new System.Windows.Forms.Button();
            this.EEpromFileName = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lockOptions = new System.Windows.Forms.Button();
            this.lockBootLoader = new System.Windows.Forms.CheckBox();
            this.lockProgram = new System.Windows.Forms.CheckBox();
            this.BLB01 = new System.Windows.Forms.CheckBox();
            this.BLB12 = new System.Windows.Forms.CheckBox();
            this.BLB11 = new System.Windows.Forms.CheckBox();
            this.BLB02 = new System.Windows.Forms.CheckBox();
            this.groupBox_comport = new System.Windows.Forms.GroupBox();
            this.errorInfoButton = new System.Windows.Forms.Button();
            this.PortCloseButton = new System.Windows.Forms.Button();
            this.twoStopBits = new System.Windows.Forms.CheckBox();
            this.PortOpenButton = new System.Windows.Forms.Button();
            this.RTS = new System.Windows.Forms.CheckBox();
            this.DTR = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.CommSpeed = new System.Windows.Forms.ComboBox();
            this.PortSelect = new System.Windows.Forms.ComboBox();
            this.button_background = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_crystal = new System.Windows.Forms.TextBox();
            this.SendReset = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.Device = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.PageSize = new System.Windows.Forms.Label();
            this.BootSize = new System.Windows.Forms.Label();
            this.FlashSize = new System.Windows.Forms.Label();
            this.EEpromSize = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.MessageList = new System.Windows.Forms.ListBox();
            this.messageClearButton = new System.Windows.Forms.Button();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.closePortAfterProgramming = new System.Windows.Forms.CheckBox();
            this.disableProgrammingCheckBox = new System.Windows.Forms.CheckBox();
            this.ProgressBar = new System.Windows.Forms.ProgressBar();
            this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.AboutButton = new System.Windows.Forms.Button();
            this.ToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.cpuProgrammingThread = new System.ComponentModel.BackgroundWorker();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox_comport.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.clearButtonFLASH);
            this.groupBox1.Controls.Add(this.FlashOpen);
            this.groupBox1.Controls.Add(this.FlashFileName);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(587, 41);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "FLASH Programming File (*.HEX)";
            // 
            // clearButtonFLASH
            // 
            this.clearButtonFLASH.BackColor = System.Drawing.SystemColors.ControlDark;
            this.clearButtonFLASH.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.clearButtonFLASH.Location = new System.Drawing.Point(532, 12);
            this.clearButtonFLASH.Name = "clearButtonFLASH";
            this.clearButtonFLASH.Size = new System.Drawing.Size(49, 24);
            this.clearButtonFLASH.TabIndex = 2;
            this.clearButtonFLASH.Text = "Clear";
            this.clearButtonFLASH.UseVisualStyleBackColor = false;
            this.clearButtonFLASH.Click += new System.EventHandler(this.clearFLASHFileButton);
            // 
            // FlashOpen
            // 
            this.FlashOpen.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FlashOpen.Location = new System.Drawing.Point(477, 13);
            this.FlashOpen.Name = "FlashOpen";
            this.FlashOpen.Size = new System.Drawing.Size(49, 24);
            this.FlashOpen.TabIndex = 1;
            this.FlashOpen.Text = "Open";
            this.ToolTip.SetToolTip(this.FlashOpen, "Open Flash File");
            this.FlashOpen.Click += new System.EventHandler(this.FlashOpen_Click);
            // 
            // FlashFileName
            // 
            this.FlashFileName.AllowDrop = true;
            this.FlashFileName.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FlashFileName.Location = new System.Drawing.Point(8, 15);
            this.FlashFileName.Name = "FlashFileName";
            this.FlashFileName.ReadOnly = true;
            this.FlashFileName.Size = new System.Drawing.Size(463, 23);
            this.FlashFileName.TabIndex = 0;
            this.ToolTip.SetToolTip(this.FlashFileName, "You can drag and drop your flash hex file here.");
            this.FlashFileName.DragDrop += new System.Windows.Forms.DragEventHandler(this.flashDragDrop);
            this.FlashFileName.DragEnter += new System.Windows.Forms.DragEventHandler(this.flashDragEnter);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.clearButtonEEPROM);
            this.groupBox2.Controls.Add(this.EEpromOpen);
            this.groupBox2.Controls.Add(this.EEpromFileName);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox2.Location = new System.Drawing.Point(8, 55);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(587, 42);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "AVR EEPROM File - OPTIONAL";
            // 
            // clearButtonEEPROM
            // 
            this.clearButtonEEPROM.BackColor = System.Drawing.SystemColors.ControlDark;
            this.clearButtonEEPROM.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.clearButtonEEPROM.Location = new System.Drawing.Point(532, 12);
            this.clearButtonEEPROM.Name = "clearButtonEEPROM";
            this.clearButtonEEPROM.Size = new System.Drawing.Size(49, 24);
            this.clearButtonEEPROM.TabIndex = 13;
            this.clearButtonEEPROM.Text = "Clear";
            this.clearButtonEEPROM.UseVisualStyleBackColor = false;
            this.clearButtonEEPROM.Click += new System.EventHandler(this.clearEEPROMFileButton);
            // 
            // EEpromOpen
            // 
            this.EEpromOpen.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EEpromOpen.Location = new System.Drawing.Point(477, 12);
            this.EEpromOpen.Name = "EEpromOpen";
            this.EEpromOpen.Size = new System.Drawing.Size(49, 24);
            this.EEpromOpen.TabIndex = 1;
            this.EEpromOpen.Text = "Open";
            this.ToolTip.SetToolTip(this.EEpromOpen, "Close Flash file");
            this.EEpromOpen.Click += new System.EventHandler(this.EEpromOpen_Click);
            // 
            // EEpromFileName
            // 
            this.EEpromFileName.AllowDrop = true;
            this.EEpromFileName.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EEpromFileName.Location = new System.Drawing.Point(8, 15);
            this.EEpromFileName.Name = "EEpromFileName";
            this.EEpromFileName.ReadOnly = true;
            this.EEpromFileName.Size = new System.Drawing.Size(463, 23);
            this.EEpromFileName.TabIndex = 0;
            this.ToolTip.SetToolTip(this.EEpromFileName, "You can drag and drop your eeprom hex file here.");
            this.EEpromFileName.DragDrop += new System.Windows.Forms.DragEventHandler(this.eepromDragDrop);
            this.EEpromFileName.DragEnter += new System.Windows.Forms.DragEventHandler(this.eepromDragEnter);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.lockOptions);
            this.groupBox3.Controls.Add(this.lockBootLoader);
            this.groupBox3.Controls.Add(this.lockProgram);
            this.groupBox3.Controls.Add(this.BLB01);
            this.groupBox3.Controls.Add(this.BLB12);
            this.groupBox3.Controls.Add(this.BLB11);
            this.groupBox3.Controls.Add(this.BLB02);
            this.groupBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.Location = new System.Drawing.Point(8, 109);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(303, 84);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Program Lock Bits";
            // 
            // lockOptions
            // 
            this.lockOptions.Location = new System.Drawing.Point(7, 16);
            this.lockOptions.Margin = new System.Windows.Forms.Padding(10);
            this.lockOptions.Name = "lockOptions";
            this.lockOptions.Size = new System.Drawing.Size(179, 26);
            this.lockOptions.TabIndex = 9;
            this.lockOptions.Text = "Show Advanced Lock Options";
            this.ToolTip.SetToolTip(this.lockOptions, "Click here and read the datasheet for futher information.");
            this.lockOptions.UseVisualStyleBackColor = true;
            this.lockOptions.Click += new System.EventHandler(this.lockOptions_Click);
            // 
            // lockBootLoader
            // 
            this.lockBootLoader.AutoSize = true;
            this.lockBootLoader.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lockBootLoader.Location = new System.Drawing.Point(9, 64);
            this.lockBootLoader.Name = "lockBootLoader";
            this.lockBootLoader.Size = new System.Drawing.Size(162, 17);
            this.lockBootLoader.TabIndex = 8;
            this.lockBootLoader.Text = "Lock BootLoader (Protect it!)";
            this.lockBootLoader.UseVisualStyleBackColor = true;
            this.lockBootLoader.CheckedChanged += new System.EventHandler(this.lockBootLoader_CheckedChanged);
            // 
            // lockProgram
            // 
            this.lockProgram.AutoSize = true;
            this.lockProgram.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lockProgram.Location = new System.Drawing.Point(9, 44);
            this.lockProgram.Name = "lockProgram";
            this.lockProgram.Size = new System.Drawing.Size(252, 17);
            this.lockProgram.TabIndex = 7;
            this.lockProgram.Text = "Lock Program (No further programming possible)";
            this.lockProgram.UseVisualStyleBackColor = true;
            this.lockProgram.CheckedChanged += new System.EventHandler(this.lockProgram_CheckedChanged);
            // 
            // BLB01
            // 
            this.BLB01.Location = new System.Drawing.Point(208, 16);
            this.BLB01.Name = "BLB01";
            this.BLB01.Size = new System.Drawing.Size(64, 16);
            this.BLB01.TabIndex = 6;
            this.BLB01.Text = "BLB01";
            this.BLB01.CheckedChanged += new System.EventHandler(this.BLB01_CheckedChanged);
            // 
            // BLB12
            // 
            this.BLB12.Location = new System.Drawing.Point(16, 16);
            this.BLB12.Name = "BLB12";
            this.BLB12.Size = new System.Drawing.Size(64, 16);
            this.BLB12.TabIndex = 3;
            this.BLB12.Text = "BLB12";
            this.BLB12.CheckedChanged += new System.EventHandler(this.BLB12_CheckedChanged);
            // 
            // BLB11
            // 
            this.BLB11.Location = new System.Drawing.Point(80, 16);
            this.BLB11.Name = "BLB11";
            this.BLB11.Size = new System.Drawing.Size(64, 16);
            this.BLB11.TabIndex = 4;
            this.BLB11.Text = "BLB11";
            this.BLB11.CheckedChanged += new System.EventHandler(this.BLB11_CheckedChanged);
            // 
            // BLB02
            // 
            this.BLB02.Location = new System.Drawing.Point(144, 16);
            this.BLB02.Name = "BLB02";
            this.BLB02.Size = new System.Drawing.Size(64, 16);
            this.BLB02.TabIndex = 5;
            this.BLB02.Text = "BLB02";
            this.BLB02.CheckedChanged += new System.EventHandler(this.BLB02_CheckedChanged);
            // 
            // groupBox_comport
            // 
            this.groupBox_comport.Controls.Add(this.errorInfoButton);
            this.groupBox_comport.Controls.Add(this.PortCloseButton);
            this.groupBox_comport.Controls.Add(this.twoStopBits);
            this.groupBox_comport.Controls.Add(this.PortOpenButton);
            this.groupBox_comport.Controls.Add(this.RTS);
            this.groupBox_comport.Controls.Add(this.DTR);
            this.groupBox_comport.Controls.Add(this.label3);
            this.groupBox_comport.Controls.Add(this.label2);
            this.groupBox_comport.Controls.Add(this.CommSpeed);
            this.groupBox_comport.Controls.Add(this.PortSelect);
            this.groupBox_comport.Controls.Add(this.button_background);
            this.groupBox_comport.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox_comport.Location = new System.Drawing.Point(8, 199);
            this.groupBox_comport.Name = "groupBox_comport";
            this.groupBox_comport.Size = new System.Drawing.Size(155, 193);
            this.groupBox_comport.TabIndex = 3;
            this.groupBox_comport.TabStop = false;
            this.groupBox_comport.Text = "Programming Setup";
            // 
            // errorInfoButton
            // 
            this.errorInfoButton.BackColor = System.Drawing.Color.OliveDrab;
            this.errorInfoButton.Enabled = false;
            this.errorInfoButton.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.errorInfoButton.FlatAppearance.BorderSize = 2;
            this.errorInfoButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.errorInfoButton.Location = new System.Drawing.Point(8, 152);
            this.errorInfoButton.Name = "errorInfoButton";
            this.errorInfoButton.Size = new System.Drawing.Size(142, 25);
            this.errorInfoButton.TabIndex = 16;
            this.errorInfoButton.Text = "Status";
            this.ToolTip.SetToolTip(this.errorInfoButton, "Displays general status and COM Port baud-rate error message.");
            this.errorInfoButton.UseVisualStyleBackColor = false;
            // 
            // PortCloseButton
            // 
            this.PortCloseButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.PortCloseButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PortCloseButton.Location = new System.Drawing.Point(78, 77);
            this.PortCloseButton.Name = "PortCloseButton";
            this.PortCloseButton.Size = new System.Drawing.Size(65, 25);
            this.PortCloseButton.TabIndex = 15;
            this.PortCloseButton.Text = "Close";
            this.ToolTip.SetToolTip(this.PortCloseButton, "Closes the communicaiton port.");
            this.PortCloseButton.Click += new System.EventHandler(this.closeCOMPortButton);
            // 
            // twoStopBits
            // 
            this.twoStopBits.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.twoStopBits.Location = new System.Drawing.Point(69, 133);
            this.twoStopBits.Name = "twoStopBits";
            this.twoStopBits.Size = new System.Drawing.Size(78, 16);
            this.twoStopBits.TabIndex = 13;
            this.twoStopBits.Text = "2 Stop Bits";
            this.twoStopBits.CheckedChanged += new System.EventHandler(this.twoStopBits_CheckedChanged);
            // 
            // PortOpenButton
            // 
            this.PortOpenButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PortOpenButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.PortOpenButton.Location = new System.Drawing.Point(13, 77);
            this.PortOpenButton.Name = "PortOpenButton";
            this.PortOpenButton.Size = new System.Drawing.Size(65, 25);
            this.PortOpenButton.TabIndex = 12;
            this.PortOpenButton.Text = "Open";
            this.PortOpenButton.Click += new System.EventHandler(this.PortOpenClose_Click);
            // 
            // RTS
            // 
            this.RTS.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RTS.Location = new System.Drawing.Point(9, 133);
            this.RTS.Name = "RTS";
            this.RTS.Size = new System.Drawing.Size(46, 16);
            this.RTS.TabIndex = 5;
            this.RTS.Text = "RTS";
            this.RTS.CheckedChanged += new System.EventHandler(this.RTS_CheckedChanged);
            // 
            // DTR
            // 
            this.DTR.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DTR.Location = new System.Drawing.Point(9, 114);
            this.DTR.Name = "DTR";
            this.DTR.Size = new System.Drawing.Size(64, 17);
            this.DTR.TabIndex = 4;
            this.DTR.Text = "DTR";
            this.DTR.CheckedChanged += new System.EventHandler(this.DTR_CheckedChanged);
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(80, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(64, 16);
            this.label3.TabIndex = 3;
            this.label3.Text = "Speed (bps)";
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(6, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 16);
            this.label2.TabIndex = 2;
            this.label2.Text = "PORT";
            // 
            // CommSpeed
            // 
            this.CommSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CommSpeed.DropDownWidth = 60;
            this.CommSpeed.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CommSpeed.Items.AddRange(new object[] {
            "9600",
            "38400"});
            this.CommSpeed.Location = new System.Drawing.Point(81, 44);
            this.CommSpeed.Name = "CommSpeed";
            this.CommSpeed.Size = new System.Drawing.Size(70, 21);
            this.CommSpeed.TabIndex = 1;
            // 
            // PortSelect
            // 
            this.PortSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.PortSelect.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PortSelect.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.PortSelect.Items.AddRange(new object[] {
            " None"});
            this.PortSelect.Location = new System.Drawing.Point(6, 44);
            this.PortSelect.Name = "PortSelect";
            this.PortSelect.Size = new System.Drawing.Size(70, 21);
            this.PortSelect.Sorted = true;
            this.PortSelect.TabIndex = 0;
            this.PortSelect.SelectionChangeCommitted += new System.EventHandler(this.PortSelect_SelectionChangeCommitted);
            // 
            // button_background
            // 
            this.button_background.Enabled = false;
            this.button_background.Location = new System.Drawing.Point(8, 75);
            this.button_background.Name = "button_background";
            this.button_background.Size = new System.Drawing.Size(140, 30);
            this.button_background.TabIndex = 17;
            this.button_background.Text = "button2";
            this.button_background.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(93, 41);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(43, 27);
            this.button1.TabIndex = 19;
            this.button1.Text = "Set";
            this.ToolTip.SetToolTip(this.button1, "Enter Frequency and press Set, then select the Speed(bps) to program at.");
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(8, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 17);
            this.label1.TabIndex = 18;
            this.label1.Text = "CPU Frequency";
            // 
            // textBox_crystal
            // 
            this.textBox_crystal.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox_crystal.Location = new System.Drawing.Point(8, 42);
            this.textBox_crystal.Name = "textBox_crystal";
            this.textBox_crystal.Size = new System.Drawing.Size(85, 26);
            this.textBox_crystal.TabIndex = 17;
            this.textBox_crystal.Text = "18432000";
            this.ToolTip.SetToolTip(this.textBox_crystal, "Set to your target\'s frequency");
            // 
            // SendReset
            // 
            this.SendReset.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SendReset.Location = new System.Drawing.Point(42, 20);
            this.SendReset.Name = "SendReset";
            this.SendReset.Size = new System.Drawing.Size(90, 25);
            this.SendReset.TabIndex = 10;
            this.SendReset.Text = "Send Reset";
            this.SendReset.Click += new System.EventHandler(this.SendReset_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.button1);
            this.groupBox5.Controls.Add(this.Device);
            this.groupBox5.Controls.Add(this.label1);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Controls.Add(this.textBox_crystal);
            this.groupBox5.Controls.Add(this.label7);
            this.groupBox5.Controls.Add(this.label6);
            this.groupBox5.Controls.Add(this.label5);
            this.groupBox5.Controls.Add(this.label4);
            this.groupBox5.Controls.Add(this.PageSize);
            this.groupBox5.Controls.Add(this.BootSize);
            this.groupBox5.Controls.Add(this.FlashSize);
            this.groupBox5.Controls.Add(this.EEpromSize);
            this.groupBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox5.Location = new System.Drawing.Point(169, 199);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(142, 193);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Target";
            // 
            // Device
            // 
            this.Device.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Device.ForeColor = System.Drawing.Color.Blue;
            this.Device.Location = new System.Drawing.Point(68, 78);
            this.Device.Name = "Device";
            this.Device.Size = new System.Drawing.Size(68, 16);
            this.Device.TabIndex = 5;
            this.Device.Text = "Unknown";
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(5, 168);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(62, 16);
            this.label8.TabIndex = 4;
            this.label8.Text = "EEprom:";
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(5, 102);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(62, 16);
            this.label7.TabIndex = 3;
            this.label7.Text = "Flash :";
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(5, 148);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(62, 16);
            this.label6.TabIndex = 2;
            this.label6.Text = "Boot Size :";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(5, 129);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 18);
            this.label5.TabIndex = 1;
            this.label5.Text = "Block Size";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(2, 78);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(62, 16);
            this.label4.TabIndex = 0;
            this.label4.Text = "Device:";
            // 
            // PageSize
            // 
            this.PageSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PageSize.ForeColor = System.Drawing.Color.Blue;
            this.PageSize.Location = new System.Drawing.Point(74, 131);
            this.PageSize.Name = "PageSize";
            this.PageSize.Size = new System.Drawing.Size(62, 16);
            this.PageSize.TabIndex = 6;
            this.PageSize.Text = "?";
            // 
            // BootSize
            // 
            this.BootSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BootSize.ForeColor = System.Drawing.Color.Blue;
            this.BootSize.Location = new System.Drawing.Point(74, 150);
            this.BootSize.Name = "BootSize";
            this.BootSize.Size = new System.Drawing.Size(62, 16);
            this.BootSize.TabIndex = 7;
            this.BootSize.Text = "?";
            // 
            // FlashSize
            // 
            this.FlashSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FlashSize.ForeColor = System.Drawing.Color.Blue;
            this.FlashSize.Location = new System.Drawing.Point(74, 104);
            this.FlashSize.Name = "FlashSize";
            this.FlashSize.Size = new System.Drawing.Size(59, 16);
            this.FlashSize.TabIndex = 8;
            this.FlashSize.Text = "?";
            // 
            // EEpromSize
            // 
            this.EEpromSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EEpromSize.ForeColor = System.Drawing.Color.Blue;
            this.EEpromSize.Location = new System.Drawing.Point(74, 170);
            this.EEpromSize.Name = "EEpromSize";
            this.EEpromSize.Size = new System.Drawing.Size(62, 16);
            this.EEpromSize.TabIndex = 9;
            this.EEpromSize.Text = "?";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.MessageList);
            this.groupBox6.Location = new System.Drawing.Point(317, 199);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(278, 193);
            this.groupBox6.TabIndex = 5;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Messages";
            // 
            // MessageList
            // 
            this.MessageList.Location = new System.Drawing.Point(9, 19);
            this.MessageList.Name = "MessageList";
            this.MessageList.Size = new System.Drawing.Size(259, 160);
            this.MessageList.TabIndex = 0;
            this.ToolTip.SetToolTip(this.MessageList, "Displays the programming details");
            // 
            // messageClearButton
            // 
            this.messageClearButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.messageClearButton.Location = new System.Drawing.Point(316, 20);
            this.messageClearButton.Name = "messageClearButton";
            this.messageClearButton.Size = new System.Drawing.Size(258, 25);
            this.messageClearButton.TabIndex = 1;
            this.messageClearButton.Text = "Clear Message List";
            this.ToolTip.SetToolTip(this.messageClearButton, "Clears the \"Messages\" Window");
            this.messageClearButton.UseVisualStyleBackColor = true;
            this.messageClearButton.Click += new System.EventHandler(this.messageClearButton_Click);
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.closePortAfterProgramming);
            this.groupBox8.Controls.Add(this.disableProgrammingCheckBox);
            this.groupBox8.Controls.Add(this.ProgressBar);
            this.groupBox8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox8.Location = new System.Drawing.Point(317, 109);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(278, 84);
            this.groupBox8.TabIndex = 7;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Programming Progress";
            // 
            // closePortAfterProgramming
            // 
            this.closePortAfterProgramming.AutoSize = true;
            this.closePortAfterProgramming.Location = new System.Drawing.Point(10, 62);
            this.closePortAfterProgramming.Name = "closePortAfterProgramming";
            this.closePortAfterProgramming.Size = new System.Drawing.Size(235, 20);
            this.closePortAfterProgramming.TabIndex = 11;
            this.closePortAfterProgramming.Text = "Close COM Port after Programming";
            this.closePortAfterProgramming.UseVisualStyleBackColor = true;
            this.closePortAfterProgramming.CheckedChanged += new System.EventHandler(this.closePortAfterProgramming_CheckedChanged);
            // 
            // disableProgrammingCheckBox
            // 
            this.disableProgrammingCheckBox.AutoSize = true;
            this.disableProgrammingCheckBox.Location = new System.Drawing.Point(10, 40);
            this.disableProgrammingCheckBox.Name = "disableProgrammingCheckBox";
            this.disableProgrammingCheckBox.Size = new System.Drawing.Size(158, 20);
            this.disableProgrammingCheckBox.TabIndex = 10;
            this.disableProgrammingCheckBox.Text = "Disable Programming";
            this.disableProgrammingCheckBox.UseVisualStyleBackColor = true;
            this.disableProgrammingCheckBox.CheckedChanged += new System.EventHandler(this.disableProgrammingCheckBox_CheckedChanged);
            // 
            // ProgressBar
            // 
            this.ProgressBar.BackColor = System.Drawing.Color.SkyBlue;
            this.ProgressBar.ForeColor = System.Drawing.Color.ForestGreen;
            this.ProgressBar.Location = new System.Drawing.Point(10, 18);
            this.ProgressBar.MarqueeAnimationSpeed = 50;
            this.ProgressBar.Name = "ProgressBar";
            this.ProgressBar.Size = new System.Drawing.Size(262, 14);
            this.ProgressBar.Step = 1;
            this.ProgressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.ProgressBar.TabIndex = 0;
            this.ProgressBar.Tag = "";
            // 
            // AboutButton
            // 
            this.AboutButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.AboutButton.Location = new System.Drawing.Point(170, 20);
            this.AboutButton.Name = "AboutButton";
            this.AboutButton.Size = new System.Drawing.Size(90, 25);
            this.AboutButton.TabIndex = 12;
            this.AboutButton.Text = "About";
            this.ToolTip.SetToolTip(this.AboutButton, "Read about HyperLoad and SJValley!");
            this.AboutButton.Click += new System.EventHandler(this.AboutBoutton_Click);
            // 
            // serialPort
            // 
            this.serialPort.ReadTimeout = 3000;
            this.serialPort.WriteBufferSize = 1024;
            this.serialPort.WriteTimeout = 1000;
            this.serialPort.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.Comm1_ErrorReceived);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // cpuProgrammingThread
            // 
            this.cpuProgrammingThread.DoWork += new System.ComponentModel.DoWorkEventHandler(this.cpuProgrammingThread_DoWork);
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.messageClearButton);
            this.groupBox9.Controls.Add(this.SendReset);
            this.groupBox9.Controls.Add(this.AboutButton);
            this.groupBox9.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox9.Location = new System.Drawing.Point(10, 398);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(585, 51);
            this.groupBox9.TabIndex = 8;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Miscellaneous";
            // 
            // HyperLoad
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.SystemColors.ControlLight;
            this.ClientSize = new System.Drawing.Size(596, 454);
            this.Controls.Add(this.groupBox9);
            this.Controls.Add(this.groupBox8);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox_comport);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "HyperLoad";
            this.Opacity = 0.93;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "HyperLoad 1.1 for AVR & ARM- by SJValley";
            this.Load += new System.EventHandler(this.MegaLoadForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox_comport.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
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
            try
            {
                Application.Run(new HyperLoad());
            }
            catch
            {

            }
		}

		private bool CommSetup(string portName, string Speed)
		{
            try { serialPort.Close(); }
            catch {
                //updatePort();
                return false;
            }

            bool nonStandard = true;
            int bps;

            if (portName == " None" || portName == "")
            {
                SendReset.Cursor = System.Windows.Forms.Cursors.No;

                errorInfoButton.Visible = false;
                return false;
            }
            else
            {
                serialPort.PortName = portName;
            }

                 if (Speed == " 230.4K")  bps =  230400;
            else if (Speed == " 384.0K")  bps =  384000;
            else if (Speed == " 460.8K")  bps =  460800;
            else if (Speed == " 576.0K")  bps =  576000;
            else if (Speed == " 921.6K")  bps =  921600;
            else if (Speed == "1152.0K") bps =  1152000;
            else if (Speed == " 100.0K")  bps =  100000;
            else if (Speed == " 250.0K")  bps =  250000;
            else if (Speed == " 500.0K")  bps =  500000;
            else if (Speed == "1000.0K")  bps = 1000000;
            else {
                try
                {
                    bps = Int32.Parse(Speed);
                }
                catch
                {
                    bps = 38400;
                }
                serialPort.BaudRate = bps;
                nonStandard = false;
            }

            if(nonStandard)
            {
                serialPort.BaudRate = bps;
                SendMessage("COM Port opened at non-standard " + Speed + " bps!");
            }

			try
			{
                if (DTR.Checked) serialPort.DtrEnable = true;
                else serialPort.DtrEnable = false;

                if (RTS.Checked) serialPort.RtsEnable = true;
                else serialPort.RtsEnable = false;

                serialPort.Open();
                GC.SuppressFinalize(serialPort.BaseStream);
                SendReset.Cursor = System.Windows.Forms.Cursors.Arrow;

                errorInfoButton.Visible = true;
                button_background.BackColor = Color.YellowGreen;
				return true;
			}
			catch
			{
                SendReset.Cursor = System.Windows.Forms.Cursors.No;

                errorInfoButton.Visible = false;
                button_background.BackColor = Color.IndianRed;
				return false;
			}
		}

		private byte ASCIItoHEX(char ch)
		{
			if ((ch >= '0') && (ch <= '9')) return (byte)((byte)ch - (byte)'0');
			if ((ch >= 'A') && (ch <= 'F')) return (byte)((byte)ch - (byte)'A' + 10);
			if ((ch >= 'a') && (ch <= 'f')) return (byte)((byte)ch - (byte)'a' + 10);
			return 0;
		}
        //*********************************************************************************
        // This delegate enables asynchronous calls for setting
        // the text property on a TextBox control.
        delegate void SetMessageListCallback(string Message);
		private void SendMessage(string Message)
		{
			int i = MessageList.Items.Count;
            if (this.MessageList.InvokeRequired)
            {
                SetMessageListCallback d = new SetMessageListCallback(SendMessage);
                this.Invoke(d, new object[] { Message });
            }
            else
            {
                if (Message.Length == 0)
                    MessageList.Items.Clear();
                else
                {
                    MessageList.Items.Insert(i, Message);
                    MessageList.SelectedIndex = i;
                }
            }
		}

        //*********************************************************************************
        // This delegate enables asynchronous calls for setting
        // the text property on a TextBox control.
        delegate void SetLabelTextCallback(string Message, Label label);
        private void SetLabelText(string Message, Label label)
        {
            if (label.InvokeRequired)
            {
                SetLabelTextCallback d = new SetLabelTextCallback(SetLabelText);
                this.Invoke(d, new object[] { Message, label });
            }
            else
            {
                label.Text = Message;
            }
        }

        //*********************************************************************************
        // This delegate enables asynchronous calls for setting
        // the text property on a TextBox control.
        delegate void SetProgressBarCallback(int max, int val);
        private void SetProgressBar(int max, int val)
        {
            if (this.ProgressBar.InvokeRequired)
            {
                SetProgressBarCallback d = new SetProgressBarCallback(SetProgressBar);
                this.Invoke(d, new object[] { (int)max, (int)val });
            }
            else
            {
			    ProgressBar.Maximum = max;
				ProgressBar.Value = val;
            }
        }

		private bool FillFlash()
		{
			byte MemLocHi;
			byte MemLocLo;
			int	 MemLoc;
			int  MemOffset;
			byte QteData;
			byte LocalCheckSum;
			byte CheckSum;
			byte RecType;
			int Li;
			int Ptr = 0;
			int MemUsage;
			char[] Buf = new char[512];
			int i;
			StreamReader Stream;
			string Line = "";

			try
			{
				Stream = new StreamReader(new FileStream(FlashFileNameHex,FileMode.Open,FileAccess.Read));
			}
			catch
			{
				return false;
			}

			if (Stream != System.IO.StreamReader.Null)
			{
				Li = 0;
				FlashMin = 0xffff;
				FlashMax = 0x0000;
				MemOffset = 0;

				for (i=0;i<(256*1024);i++) Flash[i] = 0xff;
				while (Ptr < 512)
				{
					LocalCheckSum = 0;
					Ptr = 0;

					Line = Stream.ReadLine();
					if (Line == null) break;
                    try
                    {
                        Buf = Line.ToCharArray();

                        if (Buf[Ptr++] != ':')
                        {
                            SendMessage("Parse error in Flash Hex File Line " + Li.ToString());
                            return false;
                        }
                    }
                    catch
                    {
                        return false;
                    }

					QteData = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
					LocalCheckSum += QteData;

					MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
					LocalCheckSum += MemLocHi;

					MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
					LocalCheckSum += MemLocLo;

					RecType = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
					LocalCheckSum += RecType;

					MemLoc = (MemLocHi << 8) + MemLocLo;

					if (RecType == 0) // data
					{
						if (FlashMin > (MemOffset + MemLoc)) FlashMin = (MemOffset + MemLoc);
						for (i=0;i<QteData;i++)
						{
                            if ((MemOffset + MemLoc + i) > Flash.Length)
                            {
                                int error = 1;
                            }
                            else
                            {
                                Flash[MemOffset + MemLoc + i] = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
                                LocalCheckSum += Flash[MemOffset + MemLoc + i];
                                if (FlashMax < (MemOffset + MemLoc + i)) FlashMax = (MemOffset + MemLoc + i);
                            }
						}
					}

					if (RecType == 1) // eof
					{
						MemUsage = FlashMax - FlashMin + 1;
                        FLASH_FILE_USAGE = MemUsage;
						SendMessage("Flash Hex File OK " + MemUsage.ToString() + " Bytes");
						Stream.Close();

                        /*
                        try
                        {
                            TextWriter flashfile = new StreamWriter("flash.txt");
                            int n = 0;
                            for (int m = FlashMin; m <= FlashMax; m++)
                            {
                                flashfile.Write("0x{0:X2}", Flash[m]);
                                flashfile.Write(",");
                                if ((++n % 16) == 0) flashfile.Write("\r\n");
                            }
                            flashfile.Flush();
                            flashfile.Close();
                        }
                        catch
                        {

                        }
                         */
						return true;
					}

					if (RecType == 2) // Extended Segment Address Record
					{
						MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocHi;

						MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocLo;

						MemOffset = ((MemLocHi << 8) + MemLocLo);
						MemOffset = (MemOffset << 4);
					}

					if (RecType == 4) // Extended Linear Address Record
					{
						MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocHi;

						MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocLo;

						MemOffset = ((MemLocHi << 8) + MemLocLo);
						MemOffset = (MemOffset << 16);
					}

					CheckSum = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
					LocalCheckSum = (byte)(0x0100 - LocalCheckSum);
					if (CheckSum != LocalCheckSum)
					{
						SendMessage("Warning: Checksum error at Line " + Li.ToString());
						//Stream.Close();
						//return false;
					}
					Li++;
				}
                if (Ptr >= 512)
                    return false;
				MemUsage = FlashMax - FlashMin + 1;
                FLASH_FILE_USAGE = MemUsage;
				SendMessage("Flash Hex File OK " + MemUsage.ToString() + " Bytes");
				Stream.Close();

				return true;
			}
			return false;
		}
		private void SendFlashPage()
		{
			int ByteSend = 0;
			byte CheckSum = 0;

			if ((PagePtr*PageSizeInt) > FlashMax)
			{
                // SendMessage("Finished sending flash data.");
				CommWriteByte(0xff);
				CommWriteByte(0xff);
                SetProgressBar(1, 0);

				PagePtr = 0;
			}
			else
			{
				CommWriteByte((byte)((PagePtr>>8) & 0x00ff));
				CommWriteByte((byte)((PagePtr & 0x00ff)));
				CheckSum = 0;

                SendMessage("Sending Block #" + PagePtr.ToString());
                try
                {
                   serialPort.Write(Flash, (PagePtr * PageSizeInt), PageSizeInt);
                   //for(int i=PagePtr*PageSizeInt; i<((PagePtr*PageSizeInt)+PageSizeInt); i++)
                       //CommWriteByte(Flash[i]);
                }
                catch
                {
                    SendMessage("Error sending Flash Data!");
                }
				while(ByteSend < PageSizeInt)
				{
					CheckSum += Flash[(PagePtr*PageSizeInt)+ByteSend];
					ByteSend++;
				}
				CommWriteByte((byte)CheckSum);

				if (PageSizeInt != 0)
				{
                    SetProgressBar((FlashMax / PageSizeInt), PagePtr);
				}
			}
            serialPort.BaseStream.Flush();
            while (serialPort.BytesToWrite != 0) ;
		}

		private bool FillEEprom()
		{
			byte MemLocHi;
			byte MemLocLo;
			int	 MemLoc;
			int  MemOffset;
			byte QteData;
			byte LocalCheckSum;
			byte CheckSum;
			byte RecType;
			int Li;
			int Ptr = 0;
			int MemUsage;
			char[] Buf = new char[512];
			int i;
			StreamReader Stream;
			string Line = "";

			try
			{
				Stream = new StreamReader(new FileStream(EEpromFileNameHex,FileMode.Open,FileAccess.Read));
			}
			catch
			{
				return false;
			}

			if (Stream != System.IO.StreamReader.Null)
			{
				Li = 0;
				EEpromMin = 0xffff;
				EEpromMax = 0x0000;
				MemOffset = 0;

				for (i=0;i<(128*1024);i++) 
				{
					EEprom[i] = 0xff;
					EEpromUse[i] = false;
				}
					while (true)
					{
						LocalCheckSum = 0;
						Ptr = 0;

						Line = Stream.ReadLine();
						if (Line == null) break;
						Buf = Line.ToCharArray();
						if (Buf[Ptr++] != ':')
						{
							SendMessage("Error in EEprom Hex File Line " + Li.ToString());
							return false;
						}

						QteData = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += QteData;

						MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocHi;

						MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += MemLocLo;

						RecType = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum += RecType;

						MemLoc = (MemLocHi << 8) + MemLocLo;

						if (MemLoc > 4096)
						{
							SendMessage("EEprom Hex File Out Of Range");
							Stream.Close();
							return false;
						}

						if (RecType == 0) // data
						{
							if (EEpromMin > (MemOffset + MemLoc)) EEpromMin = (MemOffset + MemLoc);
							for (i=0;i<QteData;i++)
							{
								EEprom[MemOffset + MemLoc + i] = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
								EEpromUse[MemOffset + MemLoc + i] = true;
								LocalCheckSum += EEprom[MemOffset + MemLoc + i];
								if (EEpromMax < (MemOffset + MemLoc + i)) EEpromMax = (MemOffset + MemLoc + i);
							}
						}

						if (RecType == 1) // eof
						{
							MemUsage = EEpromMax - EEpromMin + 1;
							SendMessage("EEprom Hex File OK " + MemUsage.ToString() + " Bytes");
							Stream.Close();
							return true;
						}

						if (RecType == 2) // Extended Segment Address Record
						{
							MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
							LocalCheckSum += MemLocHi;

							MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
							LocalCheckSum += MemLocLo;

							MemOffset = ((MemLocHi << 8) + MemLocLo);
							MemOffset = (MemOffset << 4);
						}

						if (RecType == 4) // Extended Linear Address Record
						{
							MemLocHi = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
							LocalCheckSum += MemLocHi;

							MemLocLo = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
							LocalCheckSum += MemLocLo;

							MemOffset = ((MemLocHi << 8) + MemLocLo);
							MemOffset = (MemOffset << 16);
						}

						CheckSum = (byte)((ASCIItoHEX(Buf[Ptr++]) << 4) + ASCIItoHEX(Buf[Ptr++]));
						LocalCheckSum = (byte)(0x0100 - LocalCheckSum);
						if (CheckSum != LocalCheckSum)
						{
							SendMessage("Error in EEprom Hex File " + Li.ToString());
							Stream.Close();
							return false;
						}
						Li++;
					}
					MemUsage = EEpromMax - EEpromMin + 1;
					SendMessage("EEprom Hex File OK " + MemUsage.ToString() + " Bytes");
					Stream.Close();
					return true;
				}
				return false;
			}
		private void SendEEpromByte()
		{
			byte CheckSum = 0;


			if (BytePtr > EEpromMax) // All data had been send
			{
				CommWriteByte(0xff);
				CommWriteByte(0xff);
				ProgressBar.Value = 0;

				BytePtr = 0;
			}				
			else
			{
				while((EEpromUse[BytePtr] != true) && (BytePtr < 4094)) BytePtr++;

                if (BytePtr >= 4094)
                {
                    CommWriteByte(0xff);
                    CommWriteByte(0xff);
                    SendMessage("EEprom file shows no EEPROM usage!");
                }
                else
                {
                    SendMessage("Sending Byte #" + BytePtr.ToString());
                    CommWriteByte((byte)((BytePtr >> 8) & 0x00ff));
                    CommWriteByte((byte)((BytePtr & 0x00ff)));
                    CommWriteByte((byte)EEprom[BytePtr]);

                    CheckSum = (byte)((BytePtr >> 8) & 0x00ff);
                    CheckSum += (byte)(BytePtr & 0x00ff);
                    CheckSum += (byte)EEprom[BytePtr];

                    CommWriteByte((byte)CheckSum);

                    if (EEpromSizeInt > 0)
                    {
                        SetProgressBar(EEpromMax, BytePtr);
                    }
                }
			}
            serialPort.BaseStream.Flush();
		}

		private void SendLockBit()
		{
			byte LockBit = 0x00;

			if (BLB01.Checked == true) LockBit += 0x04;
			if (BLB02.Checked == true) LockBit += 0x08;
			if (BLB11.Checked == true) LockBit += 0x10;
			if (BLB12.Checked == true) LockBit += 0x20;

			CommWriteByte((byte)LockBit);
			CommWriteByte((byte)~LockBit);
		}

		private void FlashOpen_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog.Filter = "HEX File (*.hex)|*.hex|All files (*.*)|*.*";
			OpenFileDialog.ReadOnlyChecked = true;
			OpenFileDialog.ShowDialog(this);
			string FileName = OpenFileDialog.FileName.ToString();
            tryUseFlashFile(FileName);
            SendMessage("Flash Start = " + FlashMin.ToString());

            try
            {
                TextWriter flashfile = new StreamWriter("flash.txt");
                int n = 0;
                for (int m = FlashMin; m <= FlashMax; m++)
                {
                    flashfile.Write("0x{0:X2}", Flash[m]);
                    flashfile.Write(",");
                    if ((++n % 32) == 0) flashfile.Write("\r\n");
                }
                flashfile.Flush();
                flashfile.Close();
            }
            catch
            {

            }

		}

        private void tryUseFlashFile(string FileName)
        {
            if (FileName != null && FileName != "")
            {
                FlashFileNameHex = FileName;
                SendMessage("");
                SendMessage("Flash File Selected");
                RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                Key.SetValue("FlashFileName", FileName);

                if (FillFlash())
                {
                    try
                    {
                        DateTime wt = File.GetLastWriteTime(FileName);
                        string[] tokens = FileName.Split('\\');
                        FlashFileName.Text = tokens[tokens.Length - 3] + "\\" +
                                                tokens[tokens.Length - 2] + "\\" +
                                                tokens[tokens.Length - 1];
                        FlashFileName.Text += getTimeStamp(wt);

                    }
                    catch
                    {
                        FlashFileName.Text = FileName;
                    }
                }
                else
                {
                    FlashFileName.Text = "Invalid Flash HEX file";
                }
            }
        }

        private string getTimeStamp(DateTime wt)
        {
            string s =  " (" + wt.Month.ToString() + "/" + wt.Day.ToString() + "/" + wt.Year.ToString() + " " + wt.Hour.ToString() + ":" + wt.Minute.ToString() + ")";
            return s;
        }
		private void EEpromOpen_Click(object sender, System.EventArgs e)
		{
            OpenFileDialog.Filter = "HEX/EEP File (*.eep)|*.eep|(*.hex)|*.hex|All files (*.*)|*.*";
			OpenFileDialog.ReadOnlyChecked = true;
			OpenFileDialog.ShowDialog(this);
			string FileName = OpenFileDialog.FileName.ToString();
            tryUseEEPROMFile(FileName);
		
		}

        private void tryUseEEPROMFile(string FileName)
        {
            if (FileName != null && FileName != "")
            {
                EEpromFileNameHex = FileName;
                SendMessage("EEprom File Selected");
                FillEEprom();
                RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                Key.SetValue("EEpromFileName", FileName);
            }
            try
            {
                DateTime wt = File.GetLastWriteTime(FileName);
                string[] tokens = FileName.Split('\\');
                EEpromFileName.Text = tokens[tokens.Length - 3] + "\\" +
                                        tokens[tokens.Length - 2] + "\\" +
                                        tokens[tokens.Length - 1];
                EEpromFileName.Text += getTimeStamp(wt);
            }
            catch
            {
                EEpromFileName.Text = FileName;
            }
        }

		private void PortSelect_SelectionChangeCommitted(object sender, System.EventArgs e)
		{
            if (PortSelect.SelectedItem.ToString() == " None")
            {
                try
                {
                    if (serialPort.IsOpen)
                    {
                        PortCloseButton.PerformClick();
                    }
                }
                catch
                {
                    //updatePort();
                }
            }
            else if (CommSetup(PortSelect.SelectedItem.ToString(), INITIAL_BAUD_RATE.ToString()))
			{
                RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                Key.SetValue("CommPort", PortSelect.SelectedItem.ToString());

                // Closed com port, gray the "Close" buton
                PortCloseButton.ForeColor = Color.Black;
                PortCloseButton.Cursor = System.Windows.Forms.Cursors.Arrow;

                PortOpenButton.ForeColor = Color.Gray;
                PortOpenButton.Cursor = System.Windows.Forms.Cursors.No;
			}
			else
			{
				PortSelect.SelectedIndex = 0;
				MessageBox.Show("Failed to open COM Port!\nIt is probably being used by another application.", "HyperLoad",MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                // Closed com port, gray the "Close" buton
                PortCloseButton.ForeColor = Color.Gray;
                PortCloseButton.Cursor = System.Windows.Forms.Cursors.No;

                PortOpenButton.ForeColor = Color.Black;
                PortOpenButton.Cursor = System.Windows.Forms.Cursors.Arrow;
			}
		}

		private void MegaLoadForm_Load(object sender, System.EventArgs e)
		{
            CommSpeed.SelectedIndex = 0;
			Flash = new byte[512 * 1024 * 2];
			EEprom = new byte[128 * 1024];
			EEpromUse = new bool[128 * 1024];

            timer1.Interval = 25;
            timer1.Start();
            errorInfoButton.Visible = false;

            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            SendMessage("Available COM Ports: ");
            int total = 0;
            Array.Sort(ports);
            foreach (string s in ports)
            {
                SendMessage("   " + s);
                total++;
            }
            if (total == 0)
            {
                SendMessage("     No SERIAL ports found!");
            }
            SendMessage(" ");

            // Do not show advanced options by default
            lockOptions.Visible = true;
            BLB01.Visible = false;
            BLB02.Visible = false;
            BLB11.Visible = false;
            BLB12.Visible = false;

            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload");
			if (Key == null)
			{
				FlashFileName.Text = "";
				EEpromFileName.Text = "";
				PortSelect.SelectedIndex = 0;
			}

			// Flash File Name
			try
			{
				if (Key.GetValue("FlashFileName") != null)
				{
					FlashFileNameHex = Key.GetValue("FlashFileName").ToString();
                    try
                    {
                        DateTime wt = File.GetLastWriteTime(FlashFileNameHex);
                        string[] tokens = FlashFileNameHex.Split('\\');
                        FlashFileName.Text = tokens[tokens.Length - 3] + "\\" +
                                                tokens[tokens.Length - 2] + "\\" +
                                                tokens[tokens.Length - 1];
                        FlashFileName.Text += getTimeStamp(wt);
                    }
                    catch
                    {
                        FlashFileName.Text = FlashFileNameHex;
                    }
				}
				else FlashFileName.Text = "";
			}
			catch
			{
				FlashFileName.Text = "";
			}

			// EEprom File Name
			try
			{
				if (Key.GetValue("EEpromFileName") != null)
				{
					EEpromFileNameHex = Key.GetValue("EEpromFileName").ToString();
                    try
                    {
                        DateTime wt = File.GetLastWriteTime(EEpromFileNameHex);
                        string[] tokens = EEpromFileNameHex.Split('\\');
                        EEpromFileName.Text = tokens[tokens.Length - 3] + "\\" +
                                                tokens[tokens.Length - 2] + "\\" +
                                                tokens[tokens.Length - 1];
                        EEpromFileName.Text += getTimeStamp(wt);
                    }
                    catch
                    {
                        EEpromFileName.Text = EEpromFileNameHex;
                    }
				}
				else EEpromFileName.Text = "";
			}
			catch
			{
				EEpromFileName.Text = "";
			}

			// ComPort
			try
			{
				if (Key.GetValue("CommPort") != null)
				{
					PortSelect.SelectedIndex = System.Convert.ToInt16(Key.GetValue("CommPort").ToString());
				}
				else PortSelect.SelectedIndex = 0;
			}
			catch
			{
				PortSelect.SelectedIndex = 0;
			}

            try
			{
                if (Key.GetValue("crystal") != null)
                {
                    textBox_crystal.Text = Key.GetValue("crystal").ToString();
                    CommSpeed.Items.Clear();
                    addBaudRatesBasedOnCrystal(Int32.Parse(textBox_crystal.Text.ToString()));
                    try
                    {
                        CommSpeed.SelectedIndex = (CommSpeed.Items.Count-2);
                    }
                    catch
                    {
                        CommSpeed.SelectedIndex = 0;
                    }
                }
                else
                    textBox_crystal.Text = "18432000";
			}
			catch
			{
                textBox_crystal.Text = "18432000";
			}

            
			try
			{
				if (Key.GetValue("CommDTR") != null)
				{
					DTR.Checked = System.Convert.ToBoolean(Key.GetValue("CommDTR").ToString());
					//if (Comm1.IsOpen == true)
					{
                        if (DTR.Checked) serialPort.DtrEnable = true;
                        else serialPort.DtrEnable = false;
					}
				}
				if (Key.GetValue("CommRTS") != null)
				{
					RTS.Checked = System.Convert.ToBoolean(Key.GetValue("CommRTS").ToString());
                    //if (Comm1.IsOpen == true)
					{
                        if (RTS.Checked) serialPort.RtsEnable = true;
                        else serialPort.RtsEnable = false;
					}
				}
                if (Key.GetValue("STOPBIT") != null)
				{
					bool TwoStopBits = System.Convert.ToBoolean(Key.GetValue("STOPBIT").ToString());
                    //if (Comm1.IsOpen == true)
					{
                        if (TwoStopBits)
                        {
                            serialPort.StopBits = System.IO.Ports.StopBits.Two;
                            twoStopBits.Checked = true;
                        }
                        else
                        {
                            serialPort.StopBits = System.IO.Ports.StopBits.One;
                            twoStopBits.Checked = false;
                        }
					}                    
				}
                if (Key.GetValue("disableProgrammingCheckBox") != null)
                {
                    disableProgrammingCheckBox.Checked = System.Convert.ToBoolean(Key.GetValue("disableProgrammingCheckBox").ToString());
                }
                if (Key.GetValue("closePortAfterProgramming") != null)
                {
                    closePortAfterProgramming.Checked = System.Convert.ToBoolean(Key.GetValue("closePortAfterProgramming").ToString());
                }
			}
			catch
			{
                disableProgrammingCheckBox.Checked = false;
                closePortAfterProgramming.Checked = false;
                twoStopBits.Checked = false;
                DTR.Checked = false;
                RTS.Checked = false;
			}

			try
			{
				if (Key.GetValue("BLB12") != null)
				{
					BLB12.Checked = System.Convert.ToBoolean(Key.GetValue("BLB12").ToString());
				}
				if (Key.GetValue("BLB11") != null)
				{
					BLB11.Checked = System.Convert.ToBoolean(Key.GetValue("BLB11").ToString());
				}
				if (Key.GetValue("BLB02") != null)
				{
					BLB02.Checked = System.Convert.ToBoolean(Key.GetValue("BLB02").ToString());
				}
				if (Key.GetValue("BLB01") != null)
				{
					BLB01.Checked = System.Convert.ToBoolean(Key.GetValue("BLB01").ToString());
				}
			}
			catch
			{
                BLB12.Checked = false;
                BLB11.Checked = false;
                BLB02.Checked = false;
                BLB01.Checked = false;
			}

            try
            {
                if (Key.GetValue("LOCK_BOOTLOADER") != null)
                {
                    lockBootLoader.Checked = System.Convert.ToBoolean(Key.GetValue("LOCK_BOOTLOADER").ToString());
                }
            }
            catch
            {
                lockBootLoader.Checked = false;
            }

			try
			{
                if(!CommSetup(Key.GetValue("CommPort").ToString(), INITIAL_BAUD_RATE.ToString()))
                {
                    PortSelect.SelectedIndex = 0;

                    // COM port didn't open
                    PortCloseButton.ForeColor = Color.Gray;
                    PortCloseButton.Cursor = System.Windows.Forms.Cursors.No;

                    PortOpenButton.ForeColor = Color.Black;
                    PortOpenButton.Cursor = System.Windows.Forms.Cursors.Arrow;
                }
                else
                {
                    PortSelect.SelectedIndex = PortSelect.FindString(Key.GetValue("CommPort").ToString());

                    SendMessage(serialPort.PortName + " opened successfully "); //at " + INITIAL_BAUD_RATE.ToString() + " bps.");
                    SendMessage(" ");

                    // Opened com port, gray the "OPEN" buton
                    PortCloseButton.ForeColor = Color.Black;
                    PortCloseButton.Cursor = System.Windows.Forms.Cursors.Arrow;

                    PortOpenButton.ForeColor = Color.Gray;
                    PortOpenButton.Cursor = System.Windows.Forms.Cursors.No;

                    errorInfoButton.Visible = true;
                }
			}
			catch
			{

			}

			if (!FillFlash())
			{
				SendMessage("No Flash File...  Open file first!");
				FlashFileName.Text = "";
			}

			//ToolTip.SetToolTip(AboutButton,"Some nice thing to read!");
			ToolTip.SetToolTip(PortOpenButton,"Open PC COMM Port for programming.");
			ToolTip.SetToolTip(SendReset,"For this to work, your Program needs to detect \"$RESET\" from UART and either reset using Watchdog or Jump to Bootloader Section.");
			ToolTip.SetToolTip(DTR,"Toggle the state of DTR (could be used to reset target or provide power for optical RS485 Converter)");
			ToolTip.SetToolTip(RTS,"Toggle the state of RTS (could be used to reset target or provide power for optical RS485 Converter)");
			//ToolTip.SetToolTip(Status,"Status of the application");
			ToolTip.SetToolTip(PortSelect,"Select Comm Port to connect to target");
			ToolTip.SetToolTip(ProgressBar,"Displays progress indicator of Flash/EEPROM programming");
			ToolTip.SetToolTip(MessageList,"Application Status Information");
			ToolTip.SetToolTip(EEpromSize,"Displays Target's EEPROM size (in bytes)");
			ToolTip.SetToolTip(FlashSize,"Displays Target's FLASH size (in Kbytes)");
			ToolTip.SetToolTip(BootSize,"Displays Bootloader size (in words)");
			ToolTip.SetToolTip(PageSize,"Display Target's Page Size (in bytes)");
			ToolTip.SetToolTip(Device,"Display Target 's Device Type");
			ToolTip.SetToolTip(CommSpeed,"Select Communication speed");
			ToolTip.SetToolTip(BLB01,"BLB01->Lock bit setting");
			ToolTip.SetToolTip(BLB02,"BLB02->Lock bit setting");
			ToolTip.SetToolTip(BLB11,"BLB11->Lock bit setting");
			ToolTip.SetToolTip(BLB12,"BLB12->Lock bit setting");
			ToolTip.SetToolTip(EEpromOpen,"Open EEPROM data file to be programmed (in HEX format)");
			ToolTip.SetToolTip(FlashOpen,"Open Flash data file to be programmed (in HEX format)");
			ToolTip.SetToolTip(EEpromFileName,"Filename of  EEPROM data file to be programmed on bootload (in HEX format)");
			ToolTip.SetToolTip(FlashFileName,"Filename of Flash data file to be programmed on bootload (in HEX format)");

            ToolTip.SetToolTip(disableProgrammingCheckBox, "Disables programming so that HyperLoad will not send any data to initiate programming with CPU");
            ToolTip.SetToolTip(closePortAfterProgramming, "Closes COM port after programming finishes so HyperLoad will not interfere with your serial data");
            ToolTip.SetToolTip(errorInfoButton, "This shows if serial data is receiving error frames, usually due to baud-rate mismatch.");

            serialDataErrorCountTimer = 1500 / timer1.Interval;
            errorInfoButton.BackColor = Color.CornflowerBlue;
            errorInfoButton.Text = "SJValley";
		}

		private void DTR_CheckedChanged(object sender, System.EventArgs e)
		{
            try
            {
                if (serialPort.IsOpen)
                {
                    if (DTR.Checked) serialPort.DtrEnable = true;
                    else serialPort.DtrEnable = false;

                    RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                    if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                    Key.SetValue("CommDTR", DTR.Checked);
                }
                else
                {
                    SendMessage("Open COM port first before enabling DTR");
                }
            }
            catch
            {
                //updatePort();
            }
		}
		private void RTS_CheckedChanged(object sender, System.EventArgs e)
		{
            try
            {
                if (serialPort.IsOpen == true)
                {
                    if (RTS.Checked) serialPort.RtsEnable = true;
                    else serialPort.RtsEnable = false;

                    RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                    if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                    Key.SetValue("CommRTS", RTS.Checked);
                }
                else
                {
                    SendMessage("Open COM port first before enabling RTS");
                }
            }
            catch
            {
                //updatePort();
            }
		}

		private void SendReset_Click(object sender, System.EventArgs e)
		{
            try
            {
                if (serialPort.IsOpen)
                {
                    serialPort.BaudRate = (Int32.Parse(CommSpeed.Items[CommSpeed.SelectedIndex].ToString()));
                    serialPort.Write("$RESET\r\n");
                    System.Threading.Thread.Sleep(20);   // Wait for data to be sent
                    serialPort.BaudRate = INITIAL_BAUD_RATE;
                    SendMessage("\"$RESET\\r\\n\" has been sent.");
                }
                else
                {
                    SendMessage("Your COM Port is closed!");
                }
            }
            catch
            {
                //updatePort();
            }
		}

        private byte[] ByteToByteArray(byte val)
        {
            byte[] array = new byte[1];
            array[0] = val;
            return (array);
        }
        private void CommWriteByte(byte val)
        {
            try
            {
                serialPort.Write(ByteToByteArray(val), 0, 1);
            }
            catch
            {
                SendMessage("Error sending byte to serial port.");
            }
            serialPort.BaseStream.Flush();
        }
        private void CommWriteChar(char val)
        {
            try
            {
                serialPort.Write(ByteToByteArray((byte)val), 0, 1);
            }
            catch
            {
                SendMessage("Error sending byte to serial port.");
            }
            serialPort.BaseStream.Flush();
        }

		private void AboutBoutton_Click(object sender, System.EventArgs e)
		{
			About = new AboutForm(this,this.serialPort,Reg);
			About.Visible = true;
		}

        private void clearFLASHFileButton(object sender, EventArgs e)
        {
            FlashFileNameHex = "";
            FlashFileName.Text = "";
            SendMessage("Flash File Cleared");
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("FlashFileName", "");
        }
        private void clearEEPROMFileButton(object sender, EventArgs e)
        {
            EEpromFileNameHex = "";
            EEpromFileName.Text = "";
            SendMessage("EEprom File Cleared");
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("EEpromFileName", "");
      
        }

        private void twoStopBits_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                if (twoStopBits.Checked)
                {
                    {
                        serialPort.StopBits = System.IO.Ports.StopBits.Two;
                    }
                }
                else
                {
                    {
                        serialPort.StopBits = System.IO.Ports.StopBits.One;
                    }
                }
                RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                Key.SetValue("STOPBIT", twoStopBits.Checked);
            }
            catch
            {
                SendMessage("Could not set stop-bit setting!");
                //updatePort();
            }
        }

        private void messageClearButton_Click(object sender, EventArgs e)
        {
            SendMessage("");
        }

        private void updateClosedCommPortButtons()
        {
            //SetButtonText("COM Not Available!", errorInfoButton);

            SelectComboBoxItem(0, PortSelect);
            SetButtonCursor(System.Windows.Forms.Cursors.No, SendReset);

            SetButtonColor(Color.Gray, true, PortCloseButton);
            SetButtonCursor(System.Windows.Forms.Cursors.No, PortCloseButton);

            SetButtonColor(Color.Black, true, PortOpenButton);
            SetButtonColor(Color.IndianRed, false, button_background);
            SetButtonCursor(System.Windows.Forms.Cursors.Arrow, PortOpenButton);
            try
            {
                errorInfoButton.Visible = false;
            }
            catch { }
        }

        private void closeCOMPortButton(object sender, EventArgs e)
        {
            try
            {
                if (serialPort.IsOpen)
                {
                    serialPort.Close();
                    SendMessage("COM Port closed!");
                    SendReset.Cursor = System.Windows.Forms.Cursors.No;

                    // Closed com port, gray the "Close" buton
                    PortCloseButton.ForeColor = Color.Gray;
                    PortCloseButton.Cursor = System.Windows.Forms.Cursors.No;

                    PortOpenButton.ForeColor = Color.Black;
                    PortOpenButton.Cursor = System.Windows.Forms.Cursors.Arrow;

                    button_background.BackColor = Color.IndianRed;

                    errorInfoButton.Visible = false;
                }
                else
                {
                    updateClosedCommPortButtons();
                    SendMessage("COM Port closed with exception 1.");
                }
            }
            catch
            {
                updateClosedCommPortButtons();
                SendMessage("COM Port closed with exception 2.");
            }
            errorInfoButton.Visible = false;
        }

        private void PortOpenClose_Click(object sender, System.EventArgs e)
        {
            if (PortSelect.SelectedIndex > 0 && PortSelect.SelectedItem.ToString() != " None")
            {
                if(CommSetup(PortSelect.SelectedItem.ToString(), INITIAL_BAUD_RATE.ToString()))
                {
                    SendReset.Cursor = System.Windows.Forms.Cursors.Arrow;

                    PortCloseButton.ForeColor = Color.Black;
                    PortCloseButton.Cursor = System.Windows.Forms.Cursors.Arrow;

                    PortOpenButton.ForeColor = Color.Gray;
                    PortOpenButton.Cursor = System.Windows.Forms.Cursors.No;
                    button_background.BackColor = Color.YellowGreen;

                    errorInfoButton.Visible = true;
                }
                else
                {
                    MessageBox.Show("Failed to open COM Port!\nIt is probably being used by another application.", "HyperLoad", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                    SendReset.Cursor = System.Windows.Forms.Cursors.No;

                    // Com port didnt open, gray the "Close" buton
                    PortCloseButton.ForeColor = Color.Gray;
                    PortCloseButton.Cursor = System.Windows.Forms.Cursors.No;

                    PortOpenButton.ForeColor = Color.Black;
                    PortOpenButton.Cursor = System.Windows.Forms.Cursors.Arrow;
                    button_background.BackColor = Color.IndianRed;

                    errorInfoButton.Visible = false;
                }
            }
        }

        private void BLB12_CheckedChanged(object sender, System.EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("BLB12", BLB12.Checked);

        }
        private void BLB11_CheckedChanged(object sender, System.EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("BLB11", BLB11.Checked);
        }
        private void BLB02_CheckedChanged(object sender, System.EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("BLB02", BLB02.Checked);
        }
        private void BLB01_CheckedChanged(object sender, System.EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("BLB01", BLB01.Checked);
            if (BLB01.Checked == false && BLB02.Checked == false)
                lockProgram.Checked = false;
            else
                lockProgram.Checked = true;
        }

        private void lockProgram_CheckedChanged(object sender, EventArgs e)
        {
            if (lockProgram.Checked)
            {
                DialogResult result = MessageBox.Show("Program Lock will disable further programming\n" +
                                "You can only unlock using ISP Programmer using CHIP Erase Command. HyperLoad cannot undo this!\n\n" +
                                "Are you sure you want to continue?",
                                "HyperLoad", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
                if (result == DialogResult.Yes)
                {
                    // CHECK THE bITS IF THEY NEED TO BE PROGRAMMED -- MEANING set to 0 in Atmega
                    BLB02.Checked = false; // 1;
                    BLB01.Checked = true; // 0;
                }
                else
                {
                    lockProgram.Checked = false;
                }
            }
            else
            {
                BLB02.Checked = false; // 1;
                BLB01.Checked = false; // 1;
            }
        }
        private void lockBootLoader_CheckedChanged(object sender, EventArgs e)
        {
            if (lockBootLoader.Checked)
            {
                BLB12.Checked = false; // 1;
                BLB11.Checked = true; //0;
            }
            else
            {
                BLB12.Checked = false; //1;
                BLB11.Checked = false; //1;
            }
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("LOCK_BOOTLOADER", lockBootLoader.Checked);
        }
        private void lockOptions_Click(object sender, EventArgs e)
        {
            lockOptions.Visible = false;
            BLB01.Visible = true;
            BLB02.Visible = true;
            BLB11.Visible = true;
            BLB12.Visible = true;
            groupBox3.Text = "Program Lock Bits -   Checked = Programmed = 0";
        }

        private void disableProgrammingCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("disableProgrammingCheckBox", disableProgrammingCheckBox.Checked);
        }
        private void closePortAfterProgramming_CheckedChanged(object sender, EventArgs e)
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
            if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
            Key.SetValue("closePortAfterProgramming", closePortAfterProgramming.Checked);
        }

        //*********************************************************************************
        // This delegate enables asynchronous calls for setting
        // the text property on a button control.
        delegate void SetButtonTextCallback(string theText, Button theButton);
        private void SetButtonText(string theText, Button theButton)
        {
            if (theButton.InvokeRequired)
            {
                SetButtonTextCallback d = new SetButtonTextCallback(SetButtonText);
                this.Invoke(d, new object[] { theText, theButton });
            }
            else
            {
                theButton.Text = theText;
            }
        }

        //*********************************************************************************
        // This delegate enables asynchronous calls for setting
        // the backcolor property on a button control.
        delegate void SetButtonColorCallback(Color theColor, bool forground, Button theButton);
        private void SetButtonColor(Color theColor, bool forground, Button theButton)
        {
            if (theButton.InvokeRequired)
            {
                SetButtonColorCallback d = new SetButtonColorCallback(SetButtonColor);
                this.Invoke(d, new object[] { theColor, forground, theButton });
            }
            else
            {
                if(forground)
                    theButton.ForeColor = theColor;
                else
                    theButton.BackColor = theColor;
            }
        }

        delegate void SetButtonCursorCallback(Cursor theCursor, Button theButton);
        private void SetButtonCursor(Cursor theCursor, Button theButton)
        {
            if (theButton.InvokeRequired)
            {
                SetButtonCursorCallback d = new SetButtonCursorCallback(SetButtonCursor);
                this.Invoke(d, new object[] { theCursor, theButton });
            }
            else
            {
                theButton.Cursor = theCursor;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (serialDataErrorCountTimer > 0)
            {
                serialDataErrorCountTimer--;

                if (serialDataErrorCountTimer == 0)
                {
                    //errorInfoButton.BackColor = Color.LimeGreen;
                    SetButtonColor(Color.LimeGreen, false, errorInfoButton);
                    //errorInfoButton.Text = "OK";
                    SetButtonText("OK", errorInfoButton);
                }
            }
        }

        private void Comm1_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            // Set the baud rate error button to red with message for 300ms
            serialDataErrorCountTimer = 500 / timer1.Interval;
            //errorInfoButton.BackColor = Color.IndianRed;
            SetButtonColor(Color.IndianRed, false, errorInfoButton);
            //errorInfoButton.Text = "Baud Rate Error";
            SetButtonText("Baud Rate Error", errorInfoButton);
        }

        private void handleSerialData()
        {
            /* Protocol information
             * 
             * uC will send >      Hyperload will send <
             *                         otherwise if Hyperload doesn't send <, uC jumps to application code
             * When Hyperload sends < then uC sends full uC info as:
             *              $Atmega324:128:2048:32:2:1:1\n
             *              DeviceID:PageSize(bytes):Bootsize(bytes):FlashSize(k):EEPROMsize(bytes):EEPROM?:Lockbit?
             * Hyperload uses Pagesize as parameter to transfer pages at a time, rest are for info only
             * EEEPROM? should be 1 or 0 if bootloader supports EEPROM programming
             * Lockbit? should be 1 or 0 if bootloader supports lockbit programming
             * After that, pages and checksum data is sent from Hyperload
             * IF uC can verify checksum, it sends ! and Hyperload sends more pages
             * if not, then uC sends @ or ^ and Hyperload retries the page
             * @ means that data transfered from Hyperload failed checksum
             * ^ means that data failed checksum after flash read was verified
             * After all pages are transfered, uC can quit, or send ) to start EEPROM transfer
             * or uC can send % for lockbit transfer
             */
            byte ch;
            int bytesReceived = 0;
            int maxBytesReceivedAtOnce = 1024 * 4;

            while (serialPort.IsOpen && serialPort.BytesToRead > 0)
            {
                serialPort.ReadTimeout = 4000;
                ch = (byte)serialPort.ReadByte();
                bytesReceived++;

                if (bytesReceived > maxBytesReceivedAtOnce)
                {
                    MessageBox.Show("Excessive serial data on serial port!\n" +
                        "Your serial PORT is now closed for stability reasons\n" +
                        "Please hold reset on your source and then open COM port\n",
                        "SJValley", MessageBoxButtons.OK);
                    serialPort.DiscardInBuffer();
                    serialPort.DiscardOutBuffer();
                    PortCloseButton.PerformClick();
                }

                /* 
                 * Here's what happens
                 *
                 * Sender is going to send D0...D7 0xFF
                 * Hyperload is going to be at 38000 initially
                 * 
                 * If Target is at 38000bps, hyperload will get 0xFF (1 bit =  26uS)
                 * If target is at 19200bps, hyperload will get 0xFE (1 bit =  52uS)
                 * If target is at  9600bps, hyperload will get 0xF8 (1 bit = 104uS)
                 * If target is at  4800bps, hyperload will get 0x00 (1 bit = 208uS)
                 * 
                 * So detecting what char came in, we can lock at target's INIITAL
                 * BPS Setting.  After that, we can send target control word for
                 * another bps setting.
                 * 
                 */

                else if (ch == 0xFF || ch == 0xFE || ch == 0xF8 || ch == 0x80)
                {
                    if (disableProgrammingCheckBox.Checked == true)
                    {
                        serialDataErrorCountTimer = 1000 / timer1.Interval;
                        SetButtonColor(Color.IndianRed, false, errorInfoButton);
                        SetButtonText("Programming Disabled", errorInfoButton);
                    }
                    else
                    {
                        if (ch == 0xFF)         serialPort.BaudRate = 38400;
                        else if (ch == 0xFE)    serialPort.BaudRate = 19200;
                        else if (ch == 0xF8)    serialPort.BaudRate = 9600;
                        else if (ch == 0x80)    serialPort.BaudRate = 4800;

                        CommWriteChar((char)0x55);
                        try
                        {
                            if (0xAA != serialPort.ReadByte())
                                return;
                        }
                        catch { return; }


                        SendMessage("");
                        SendMessage("Target speed initially at " + serialPort.BaudRate.ToString() + " bps\n");
                                
                        int targetBaudRateDivider = 0;
                        try
                        {
                            int crystal = Int32.Parse(textBox_crystal.Text.ToString());
                            int selectedBaud = Int32.Parse(CommSpeed.Items[CommSpeed.SelectedIndex].ToString());
                            targetBaudRateDivider = (int)Math.Round(crystal / ((float)selectedBaud * 16)) - 1;

                            // Send new baud rate control word, then receive it to verify it
                            CommWriteChar((char)targetBaudRateDivider);
                            try
                            {
                                int data = serialPort.ReadByte();
                                if (data != targetBaudRateDivider)
                                {
                                    SendMessage("Target sync error (" + data.ToString() + "/" + targetBaudRateDivider.ToString() + ")");
                                    return;
                                }
                                else
                                {
                                    SendMessage("Target acknowledged new baud rate (" + selectedBaud.ToString() + ")");
                                }
                            }
                            catch
                            {
                                SendMessage("Target failed to acknowledge new baud rate.");
                                return;
                            }
                            serialPort.BaudRate = selectedBaud;
                        }
                        catch
                        {
                            SendMessage("Internal error while changing baud rate.");
                        }

                        try
                        {
                            ch = (byte)serialPort.ReadByte();
                        }
                        catch
                        {
                            SendMessage("Target failed to send its header.");
                            SendMessage("Double check Target frequency!");
                            return;
                        }
                        if ('$' == ch)
                        {
                            this.Enabled = false;
                            try
                            {
                                if (performProgramming() && closePortAfterProgramming.Checked)
                                {
                                    this.Enabled = true;
                                    PortCloseButton.PerformClick();
                                }
                                serialPort.BaudRate = INITIAL_BAUD_RATE;
                            }
                            catch { }
                        }
                        else
                        {
                            SendMessage("Unexpected target header.");
                            SendMessage("Double check Target frequency!");
                        }
                        this.Enabled = true;
                    }
                }
            } // whiles bytestoread
        }
        private bool performProgramming()
        {
            if (FlashFileName.Text == "" || !FillFlash())
            {
                try
                {
                    SendMessage("Invalid or no Flash File to program!");
                    CommWriteByte(0xff);
                    CommWriteByte(0xff);
                    serialPort.ReadByte();
                }
                catch { }
                return false;
            }
            else
            {
                try
                {
                    DateTime wt = File.GetLastWriteTime(FlashFileNameHex);
                    string[] fileTokens = FlashFileNameHex.Split('\\');
                    FlashFileName.Text = fileTokens[fileTokens.Length - 3] + "\\" +
                                            fileTokens[fileTokens.Length - 2] + "\\" +
                                            fileTokens[fileTokens.Length - 1];
                    FlashFileName.Text += getTimeStamp(wt);
                }
                catch
                {
                    FlashFileName.Text = FlashFileNameHex;
                }
            }

            string cpuDescriptionString = "";
            try
            {
                cpuDescriptionString = serialPort.ReadLine();
            }
            catch
            {
                return false;
            }

            string[] tokens = cpuDescriptionString.Split(new char[] { ':' });
            // Tokens should be like:
            // Atmega324:128:2048:32:2:1:1\n
            // DeviceID:PageSize(bytes):Bootsize(bytes):FlashSize(k):EEPROMsize(bytes):EEPROM Support:Lockbit Support\n
            if (tokens.Length != 7)
            {
                SendMessage("Failure! Invalid bootloader description.");
                return false;
            }

            try
            {
                PageSizeInt = Int32.Parse(tokens[1]);
                // Bootloader C atmel software sends size in words
                BootSizeInt = Int32.Parse(tokens[2]) * 2;
                FlashSizeInt = Int32.Parse(tokens[3]) * 1024;
                EEpromSizeInt = Int32.Parse(tokens[4]);
                try
                {
                    eepromProgrammingSupportedByCPU = Int32.Parse(tokens[5]);
                    lockBitProgrammingSupportedByCPU = Int32.Parse(tokens[6]);
                }
                catch { }
            }
            catch
            {
                SendMessage("Parse failure for CPU description string!");
                return false;
            }

            try
            {
                SetLabelText(tokens[0], Device);
                SetLabelText(tokens[4] + " bytes", EEpromSize);
                SetLabelText(tokens[3] + "K bytes", FlashSize);
                SetLabelText(BootSizeInt.ToString() + " bytes", BootSize);
                SetLabelText(tokens[1] + " bytes", PageSize);
            }
            catch
            {
            }

            bool armCpu = tokens[0].Contains("LPC");
            if (armCpu && FlashMin == 0)
            {
                MessageBox.Show("Cannot load this file!\n" +
                                "Your ARM7 LPC Hex file starts from address 0.\n" +
                                "You need to modify your loader (.ld) file such that\n" +
                                "your program is compiled with flash offset for your\n" +
                                "program to load correctly.",
                    "HyperLoad", MessageBoxButtons.OK);
                CommWriteByte(0xff);
                CommWriteByte(0xff);
                return false;
            }

            if (FLASH_FILE_USAGE >= (FlashSizeInt - BootSizeInt))
            {
                int usage = (FlashSizeInt - BootSizeInt);
                MessageBox.Show("Error: Your flash size exceeds the available flash!" +
                    "Flash available: " + usage.ToString() + " Bytes (without bootloader)\n" +
                    "Flash usage    : " + FLASH_FILE_USAGE.ToString() + "\n\n" +
                    "HyperLoad will not program target because this will erase your bootloader.\n",
                    "HyperLoad", MessageBoxButtons.OK);
                return false;
            }
            else
            {
                PagePtr = 0;
                Retry = 0;

                if (armCpu)
                {
                    groupBox3.Text = "ARM Processors do not support lockbits.";
                    lockOptions.Visible = false;
                    lockProgram.Visible = false;
                    lockBootLoader.Visible = false;
                }
                else
                {
                    if (eepromProgrammingSupportedByCPU != 1)
                        SendMessage("EEPROM Programming not supported by CPU.");
                    if (lockBitProgrammingSupportedByCPU != 1)
                        SendMessage("Lockbit Programming not supported by CPU.");
                }
            }

            char ch = 'X';
            Retry = 0;
            SendMessage("Begin FLASH Programming...");
            while(Retry <= 5)
            {
                try 
                {
                    ch = (char)serialPort.ReadByte();
                }
                catch 
                {
                    SendMessage("Programming Failed due to timeout!");
                    return false;
                }

                if ((ch == '!'))
                {
                    SendFlashPage();
                    PagePtr++;
                }
                else if ((ch == '@'))
                {
                    SendMessage("Transmission Checksum Failed!");
                    PagePtr--;
                    Retry++;
                    SendFlashPage();
                    PagePtr++;
                }
                else if ((ch == '^'))
                {
                    SendMessage(Device.Text.ToString() + " failed flash read checksum.");
                    PagePtr--;
                    Retry++;
                    SendFlashPage();
                    PagePtr++;
                }
                else 
                {
                    // this is ok --> could be "*" SendMessage("Unexpected value returned by target.");
                    break;
                }
            }//while

            if (Retry > 5)
            {
                SendMessage("Flash Programming Failed!");
                int i = 5;
                while (--i > 0 && serialPort.BytesToRead > 0)
                {
                    CommWriteByte(0xff);
                    CommWriteByte(0xff);
                    try
                    {
                        serialPort.ReadByte();
                    }
                    catch { }
                }
                return false;
            }

            if (isProgrammingFinished(ch))
                return true;

            if (ch == ')') // EEprom prog code
            {
                if (FillEEprom())
                {
                    BytePtr = 0;
                    Retry = 0;
                    SendMessage("Begin EEPROM Programming..."); 
                    try
                    {
                        DateTime wt = File.GetLastWriteTime(EEpromFileNameHex);
                        string[] fileTokens = EEpromFileNameHex.Split('\\');
                        EEpromFileName.Text = fileTokens[fileTokens.Length - 3] + "\\" +
                                                fileTokens[fileTokens.Length - 2] + "\\" +
                                                fileTokens[fileTokens.Length - 1];
                        EEpromFileName.Text += getTimeStamp(wt);
                    }
                    catch
                    {
                        EEpromFileName.Text = EEpromFileNameHex;
                    }
                }
                else
                {
                    SendMessage("No EEPROM to program...");
                    CommWriteByte(0xff);
                    CommWriteByte(0xff);

                    // Bootloader will send ! after ), so ignore it
                    char nextChar = 'X';
                    try
                    {
                        serialPort.ReadByte();
                        nextChar = (char)serialPort.ReadByte();
                    }
                    catch { }
                    if (isProgrammingFinished(nextChar))
                        return true;
                }
            }

            Retry = 0;
            while(Retry <= 5)
            {
                try
                {
                    ch = (char)serialPort.ReadByte();
                }
                catch
                {
                    SendMessage("Programming failed due to timeout!");
                    return false;
                }
                if ((ch == '!'))
                {
                    SendEEpromByte();
                    BytePtr++;
                }
                else if ((ch == '@'))
                {
                    SendMessage("EEPROM transmission Checksum failed!");
                    BytePtr--;
                    Retry++;
                    SendEEpromByte();
                    BytePtr++;

                }
                else if ((ch == '^'))
                {
                    SendMessage(Device.Text.ToString() + " failed eeprom verify checksum.");
                    BytePtr--;
                    Retry++;
                    SendEEpromByte();
                    BytePtr++;
                }
                else if (isProgrammingFinished(ch))
                    return true;
            }

            if (Retry > 5)
            {
                SendMessage("EEPROM Programming Failed!");
                int i = 5;
                while (--i > 0 && serialPort.BytesToRead > 0)
                {
                    CommWriteByte(0xff);
                    CommWriteByte(0xff);
                    try
                    {
                        serialPort.ReadByte();
                    }
                    catch { }
                }
                return false;
            }
            return true;
        }

        private bool isProgrammingFinished(char ch)
        {
            if (ch == '*')
            {
                SendMessage("Programming completed succuessfully.");
                return true;
            }
            // If after flash programming, Lockbit character was sent, then perform lockbit and exit
            else if (ch == '%')
            {
                SendLockBit();
                SendMessage("Programming completed successfully.");
                SendMessage("Lockbits sent...");
                return true;
            }
            return false;
        }
        private void updatePort()
        {
            try
            {
                if (serialPort.IsOpen)
                {
                }
            }
            catch
            {
                addUpdatePorts();
            }
           
        }

        delegate void AddComboBoxItemCallback(string theItem, ComboBox theBox);
        private void AddComboBoxItem(string theItem, ComboBox theBox)
        {
            if (theBox.InvokeRequired)
            {
                AddComboBoxItemCallback d = new AddComboBoxItemCallback(AddComboBoxItem);
                this.Invoke(d, new object[] { theItem, theBox });
            }
            else
            {
                if(theItem == " " )
                    theBox.Items.Clear();
                else 
                    theBox.Items.Add(theItem);
            }
        }

        delegate void SelectComboBoxItemCallback(int theItem, ComboBox theBox);
        private void SelectComboBoxItem(int theItem, ComboBox theBox)
        {
            if (theBox.InvokeRequired)
            {
                SelectComboBoxItemCallback d = new SelectComboBoxItemCallback(SelectComboBoxItem);
                this.Invoke(d, new object[] { theItem, theBox });
            }
            else
            {
                theBox.SelectedIndex = theItem;
            }
        }
        
        private bool addUpdatePorts()
        {
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            int totalPorts = 1; // " None" is 1 of them
            foreach (string s in ports)
            {
                totalPorts++;
            }

            if (totalPorts != PortSelect.Items.Count)
            {
                Array.Sort(ports);
                AddComboBoxItem(" ", PortSelect);
                AddComboBoxItem(" None", PortSelect);

                foreach (string s in ports)
                {
                    AddComboBoxItem(s, PortSelect);
                }
                SelectComboBoxItem(0, PortSelect);

                SendMessage("");
                SendMessage("----------------------------------------------------------");
                SendMessage("COM Ports have changed!");
                SendMessage("COM Ports available:");
                SendMessage("----------------------------------------------------------");

                totalPorts = 0;
                foreach (string s in ports)
                {
                    totalPorts++;
                    SendMessage(totalPorts.ToString() + ".  " + s);
                }
                SendMessage("----------------------------------------------------------");

                return true;
            }
            return false;
        }
        
        private void cpuProgrammingThread_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                try
                {
                    if(serialPort.IsOpen && serialPort.BytesToRead > 0)
                    {
                        handleSerialData();
                    }
                    else
                    {
                        Thread.Sleep(1);
                        if ((++portUpdateTimer % 500) == 0)
                        {
                            if (addUpdatePorts())
                            {
                                SendMessage("Your COMM Port is closed ");
                                updateClosedCommPortButtons();
                            }
                        }
                    }
                }
                catch
                {
                }                
            }
        }

        float getBaudErrorPercentage(int crystal, int baud)
        {
            int intDiv = (int)Math.Round(crystal / ((float)baud * 16)) - 1;
            if (intDiv == -1)
                return 100;
            int actualBaudRate = (crystal / (16 * (intDiv + 1)));

            return (Math.Abs(actualBaudRate - baud) / (float)baud) * 100;
        }

        private bool tryAddBuadRate(int crystal, int tryBaud)
        {
            if (getBaudErrorPercentage(crystal, tryBaud) < 3)
            {
                CommSpeed.Items.Add(tryBaud.ToString());
                return true;
            }
            return false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int crystal;
            try
            {
                crystal = Int32.Parse(textBox_crystal.Text);
            }
            catch
            {
                crystal = 18432000;
            }
            if (crystal == 0)
                crystal = 18432000;

            textBox_crystal.Text = crystal.ToString();
            CommSpeed.Items.Clear();
            if (!addBaudRatesBasedOnCrystal(crystal))
            {
                textBox_crystal.Text = "18432000";
                MessageBox.Show("Your target does not support at least 4800bps.\nHyperLoad cannot program at the selected frequency.", "HyperLoad", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                
                button1.PerformClick();
                return;
            }

            try
            {
                CommSpeed.SelectedIndex = 0;
                RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\SJValley\\Hyperload", true);
                if (Key == null) Key = Registry.CurrentUser.CreateSubKey("Software\\SJValley\\Hyperload");
                Key.SetValue("crystal", textBox_crystal.Text);

                try
                {
                    CommSpeed.SelectedIndex = (CommSpeed.Items.Count-2);
                }
                catch
                {
                    CommSpeed.SelectedIndex = 0;
                }
            }
            catch
            {
                textBox_crystal.Text = "18432000";
                MessageBox.Show("Your target doesn't support any valid baud rate at the crystal speed selected!", "HyperLoad", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                button1.PerformClick();
            }
        }

        private bool addBaudRatesBasedOnCrystal(int crystal)
        {
            // Need support for at least 4800bps
            if (!tryAddBuadRate(crystal, 4800))
                return false;

            tryAddBuadRate(crystal, 9600);
            tryAddBuadRate(crystal, 19200);
            tryAddBuadRate(crystal, 38400);
            tryAddBuadRate(crystal, 57600);
            tryAddBuadRate(crystal, 72000);
            tryAddBuadRate(crystal, 100000);
            tryAddBuadRate(crystal, 115200);
            tryAddBuadRate(crystal, 125000);
            tryAddBuadRate(crystal, 144000);
            tryAddBuadRate(crystal, 230400);
            tryAddBuadRate(crystal, 250000);
            tryAddBuadRate(crystal, 500000);
            tryAddBuadRate(crystal, 576000);
            tryAddBuadRate(crystal, 1152000);
            tryAddBuadRate(crystal, 1000000);
            return true;
        }

        private void flashDragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop, false) == true)
                // allow them to continue
                // (without this, the cursor stays a "NO" symbol
                e.Effect = DragDropEffects.All;
        }

        private void eepromDragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop, false) == true)
                // allow them to continue
                // (without this, the cursor stays a "NO" symbol
                e.Effect = DragDropEffects.All;
        }

        private void flashDragDrop(object sender, DragEventArgs e)
        {
            // transfer the filenames to a string array
            // (yes, everything to the left of the "=" can be put in the 
            // foreach loop in place of "files", but this is easier to understand.)
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

            string file = "";
            if (files.Length > 0)
            {
                try
                {
                    file = files[0];
                    string s = file.Remove(0, file.Length - 3);
                    s = s.ToUpper();
                    if (s == "HEX")
                    {
                        tryUseFlashFile(file);
                    }
                    else
                    {
                        FlashFileName.Text = "You can only drag and drop *.HEX files!";
                        FlashFileNameHex = "";
                    }
                }
                catch
                {
                    FlashFileName.Text = "You can only drag and drop *.HEX files!";
                    FlashFileNameHex = "";
                }
            }
        }

        private void eepromDragDrop(object sender, DragEventArgs e)
        {
            // transfer the filenames to a string array
            // (yes, everything to the left of the "=" can be put in the 
            // foreach loop in place of "files", but this is easier to understand.)
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

            string file = "";
            if (files.Length > 0)
            {
                try
                {
                    file = files[0];
                    string s = file.Remove(0, file.Length - 3);
                    s = s.ToUpper();
                    if (s == "EEP")
                    {
                        tryUseEEPROMFile(file);
                    }
                    else
                    {
                        EEpromFileName.Text = "You can only drag and drop *.EEP files!";
                        EEpromFileNameHex = "";
                    }
                }
                catch
                {
                    EEpromFileName.Text = "You can only drag and drop *.EEP files!";
                    EEpromFileNameHex = "";
                }
            }
        }
	}
}
