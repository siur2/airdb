#pragma once

#include "ImportController.h"

namespace AirDBImport {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainForm : public Form
	{
	private:
		ImportController* importController;
	public:
		MainForm(void)
		{
			InitializeComponent();
			
			importController = new ImportController();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}

			if (importController) {
				delete importController;
				importController = 0;
			}
		}
	private: System::Windows::Forms::Button^  btnMongoImport;
	private: System::Windows::Forms::Label^  lblConnectionString;
	private: System::Windows::Forms::TextBox^  txtConnectionString;
	private: System::Windows::Forms::Label^  lblTableName;
	private: System::Windows::Forms::TextBox^  txtTableName;
	private: System::Windows::Forms::TextBox^  txtTableMetadata;





	private: System::Windows::Forms::Label^  lblTableMetadata;

	private: System::Windows::Forms::Label^  lblRecordsLimit;
	private: System::Windows::Forms::TextBox^  txtRecordsLimit;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->btnMongoImport = (gcnew System::Windows::Forms::Button());
			this->lblConnectionString = (gcnew System::Windows::Forms::Label());
			this->txtConnectionString = (gcnew System::Windows::Forms::TextBox());
			this->lblTableName = (gcnew System::Windows::Forms::Label());
			this->txtTableName = (gcnew System::Windows::Forms::TextBox());
			this->txtTableMetadata = (gcnew System::Windows::Forms::TextBox());
			this->lblTableMetadata = (gcnew System::Windows::Forms::Label());
			this->lblRecordsLimit = (gcnew System::Windows::Forms::Label());
			this->txtRecordsLimit = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// btnMongoImport
			// 
			this->btnMongoImport->Location = System::Drawing::Point(400, 249);
			this->btnMongoImport->Name = L"btnMongoImport";
			this->btnMongoImport->Size = System::Drawing::Size(75, 23);
			this->btnMongoImport->TabIndex = 0;
			this->btnMongoImport->Text = L"Import";
			this->btnMongoImport->UseVisualStyleBackColor = true;
			this->btnMongoImport->Click += gcnew System::EventHandler(this, &MainForm::btnMongoImport_Click);
			// 
			// lblConnectionString
			// 
			this->lblConnectionString->AutoSize = true;
			this->lblConnectionString->Location = System::Drawing::Point(24, 21);
			this->lblConnectionString->Name = L"lblConnectionString";
			this->lblConnectionString->Size = System::Drawing::Size(89, 13);
			this->lblConnectionString->TabIndex = 1;
			this->lblConnectionString->Text = L"Connection string";
			// 
			// txtConnectionString
			// 
			this->txtConnectionString->Location = System::Drawing::Point(128, 18);
			this->txtConnectionString->Name = L"txtConnectionString";
			this->txtConnectionString->Size = System::Drawing::Size(347, 20);
			this->txtConnectionString->TabIndex = 2;
			this->txtConnectionString->Text = L"Server=SSIREDZHUK-PC\\SQLEXPRESS; Database=urs; Integrated Security=True";
			// 
			// lblTableName
			// 
			this->lblTableName->AutoSize = true;
			this->lblTableName->Location = System::Drawing::Point(24, 48);
			this->lblTableName->Name = L"lblTableName";
			this->lblTableName->Size = System::Drawing::Size(63, 13);
			this->lblTableName->TabIndex = 3;
			this->lblTableName->Text = L"Table name";
			// 
			// txtTableName
			// 
			this->txtTableName->Location = System::Drawing::Point(128, 45);
			this->txtTableName->Name = L"txtTableName";
			this->txtTableName->Size = System::Drawing::Size(347, 20);
			this->txtTableName->TabIndex = 4;
			this->txtTableName->Text = L"RealMoneyBets";
			// 
			// txtTableMetadata
			// 
			this->txtTableMetadata->Location = System::Drawing::Point(128, 97);
			this->txtTableMetadata->Multiline = true;
			this->txtTableMetadata->Name = L"txtTableMetadata";
			this->txtTableMetadata->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtTableMetadata->Size = System::Drawing::Size(347, 137);
			this->txtTableMetadata->TabIndex = 7;
			this->txtTableMetadata->Text = resources->GetString(L"txtTableMetadata.Text");
			// 
			// lblTableMetadata
			// 
			this->lblTableMetadata->AutoSize = true;
			this->lblTableMetadata->Location = System::Drawing::Point(24, 100);
			this->lblTableMetadata->Name = L"lblTableMetadata";
			this->lblTableMetadata->Size = System::Drawing::Size(81, 13);
			this->lblTableMetadata->TabIndex = 8;
			this->lblTableMetadata->Text = L"Table metadata";
			// 
			// lblRecordsLimit
			// 
			this->lblRecordsLimit->AutoSize = true;
			this->lblRecordsLimit->Location = System::Drawing::Point(24, 74);
			this->lblRecordsLimit->Name = L"lblRecordsLimit";
			this->lblRecordsLimit->Size = System::Drawing::Size(67, 13);
			this->lblRecordsLimit->TabIndex = 9;
			this->lblRecordsLimit->Text = L"Records limit";
			// 
			// txtRecordsLimit
			// 
			this->txtRecordsLimit->Location = System::Drawing::Point(128, 71);
			this->txtRecordsLimit->Name = L"txtRecordsLimit";
			this->txtRecordsLimit->Size = System::Drawing::Size(347, 20);
			this->txtRecordsLimit->TabIndex = 10;
			this->txtRecordsLimit->Text = L"0";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(506, 284);
			this->Controls->Add(this->txtRecordsLimit);
			this->Controls->Add(this->lblRecordsLimit);
			this->Controls->Add(this->lblTableMetadata);
			this->Controls->Add(this->txtTableMetadata);
			this->Controls->Add(this->txtTableName);
			this->Controls->Add(this->lblTableName);
			this->Controls->Add(this->txtConnectionString);
			this->Controls->Add(this->lblConnectionString);
			this->Controls->Add(this->btnMongoImport);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"AirDB Import Tool";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnMongoImport_Click(System::Object^  sender, System::EventArgs^  e) {
		int recordsLimit = Int32::Parse(txtRecordsLimit->Text);
		long long count = importController->import(
			txtConnectionString->Text,
			txtTableName->Text,
			txtTableMetadata->Text,
			recordsLimit);
		MessageBox::Show(L"RealMoneyBets record count = " + count);
	}

};
}
