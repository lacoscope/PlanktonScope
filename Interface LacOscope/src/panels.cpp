#include "frame.h"


#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

string PYTHON_PATH=getexepath().append("/python");

IdPanel::IdPanel(wxPanel *parent):wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
	parent_panel=parent;
	path_title=new wxStaticText(this,-1,wxT("Sélection de l'image à identifier"),wxPoint(10,10),wxSize(420,30),wxST_ELLIPSIZE_MIDDLE | wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);

	path_select=new wxButton(this,ID_PATH,wxT("Select"),wxPoint(320,39),wxSize(100,22));
	path_text=new wxStaticText(this,-1,wxT("Select a path"),wxPoint(10,40),wxSize(300,20),wxST_ELLIPSIZE_MIDDLE | wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);
	
	run_button=new wxButton(this,ID_RUN_IDENTIF,wxT("Run !"),wxPoint(10,70),wxSize(410,20));

	console=new wxTextCtrl(this,-1,wxT(""), wxPoint(10,110), wxSize(410,200),wxTE_READONLY | wxTE_NO_VSCROLL | wxTE_BESTWRAP | wxTE_MULTILINE);


	path_select->SetOwnBackgroundColour(MEDIUM_GRAY);
	path_text->SetOwnBackgroundColour(MEDIUM_GRAY);
	run_button->SetOwnBackgroundColour(LIGHT_GREEN);

	
	Connect(ID_PATH,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(IdPanel::select_file));
	Connect(ID_RUN_IDENTIF,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(IdPanel::run));
	
	//setenv("PYTHONPATH","./python",1);
	//Py_Initialize();
	PyObject* pName = PyUnicode_FromString((char*)"utils");
	ut = PyImport_Import(pName);
	PyObject* pName2 = PyUnicode_FromString((char*)"identification");
	id = PyImport_Import(pName2);
	if(id==NULL){
		cout << "echec import module id"<<endl;
	}
	
}

void IdPanel::select_file(wxCommandEvent & WXUNUSED(event))
{
	wxFileDialog * openFileDialog = new wxFileDialog(this);
	if (openFileDialog->ShowModal() == wxID_OK){
		wxString p=openFileDialog->GetPath();
		this->path_text->SetLabel(p);
		this->path=new wxString(p);
	}
}


bool IdPanel::ShouldInheritColours()const{
	return false;
}

void IdPanel::run(wxCommandEvent & WXUNUSED(event))
{
	wxStreamToTextRedirector redirect(console);
	cout << "Run"<<endl;
	Fenetre *f=(Fenetre *)this->parent_panel->GetParent();
	if(this->path){
		if(this->id)
		{
			PyObject* pFunc = PyObject_GetAttrString(id, "prediction");
			if(pFunc && PyCallable_Check(pFunc))
			{
				cout << (const char*)this->path->mb_str() << endl;
				PyObject* arg1=PyUnicode_FromString((const char*)this->path->mb_str());
				PyObject* args=PyTuple_New(1);
				PyTuple_SetItem(args,0,arg1);
				
				PyObject *pValue = PyObject_CallObject(pFunc,args);
				
				
				const char *s=PyUnicode_AsUTF8(pValue);
				cout<<s<<endl;
				f->id_disp_p->update_img();
			}
			else cout << "Echec chargement fonction"<<endl;
		}
		else cout <<"Echec chargement script"<<endl;
	}
	else cout << "Veuillez selectionner un fichier"<<endl;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MvtPanel::MvtPanel(wxPanel *parent):wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
	parent_panel=parent;
	
	path_title=new wxStaticText(this,-1,wxT("Sélection des deux images"),wxPoint(10,10),wxSize(420,30),wxST_ELLIPSIZE_MIDDLE | wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);

	path_select=new wxButton(this,ID_PATH,wxT("Select"),wxPoint(320,39),wxSize(100,22));
	path_text=new wxStaticText(this,-1,wxT("Select a path"),wxPoint(10,40),wxSize(300,20),wxST_ELLIPSIZE_MIDDLE | wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);
	
	files_list=new wxStaticText(this,-1,wxT(""),wxPoint(10,70),wxSize(410,40),wxST_ELLIPSIZE_MIDDLE | wxALIGN_LEFT);

	run_button=new wxButton(this,ID_RUN_MVT,wxT("Run !"),wxPoint(10,120),wxSize(410,20));
	
	console=new wxTextCtrl(this,-1,wxT(""), wxPoint(10,170), wxSize(410,200),wxTE_READONLY | wxTE_NO_VSCROLL | wxTE_BESTWRAP | wxTE_MULTILINE);
	



	path_select->SetOwnBackgroundColour(MEDIUM_GRAY);
	path_text->SetOwnBackgroundColour(MEDIUM_GRAY);
	files_list->SetOwnBackgroundColour(MEDIUM_GRAY);
	run_button->SetOwnBackgroundColour(LIGHT_GREEN);
	
	Connect(ID_PATH,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MvtPanel::select_path));
	Connect(ID_RUN_MVT,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MvtPanel::run));
		
	
	setenv("PYTHONPATH","./python",1);
	Py_Initialize();
	PyObject* pName = PyUnicode_FromString("utils");
	ut = PyImport_Import(pName);
	if(ut==NULL){
		cout << "echec import module ut"<<endl;
	}
	
	pName= PyUnicode_FromString((char*)"mouvement");

	mvt = PyImport_Import(pName);
	if(mvt==NULL){
		cout << "echec import module mvt"<<endl;
	}
	
}


void MvtPanel::select_path(wxCommandEvent & WXUNUSED(event))
{
	wxFileDialog * openFileDialog = new wxFileDialog(this,wxFileSelectorPromptStr,wxEmptyString,wxEmptyString,wxFileSelectorDefaultWildcardStr,wxFD_MULTIPLE|wxFD_PREVIEW);
	if (openFileDialog->ShowModal() == wxID_OK){
		wxArrayString p;
		openFileDialog->GetFilenames(p);
		this->path_text->SetLabel(openFileDialog->GetDirectory());
		this->liste_fichiers(p);
		
		openFileDialog->GetPaths(p);
		this->filespath=p;
	}
}



void MvtPanel::run(wxCommandEvent & WXUNUSED(event))
{
	wxStreamToTextRedirector redirect(console);
	cout << "Run"<<endl;
	Fenetre *f=(Fenetre *)this->parent_panel->GetParent();
	if(this->filespath.GetCount()==2){
		wxArrayString p(this->filespath);
		if(this->mvt)
		{
			PyObject* pFunc = PyObject_GetAttrString(mvt, "estimation_mouvement");
			if(pFunc && PyCallable_Check(pFunc))
			{
				cout << (const char*)p.Item(0).mb_str() << endl;
				cout << (const char*)p.Item(1).mb_str() << endl;
				PyObject* arg1=PyUnicode_FromString((const char*)p.Item(0).mb_str());
				PyObject* arg2=PyUnicode_FromString((const char*)p.Item(1).mb_str());
				PyObject* args=PyTuple_New(2);
				PyTuple_SetItem(args,0,arg1);
				PyTuple_SetItem(args,1,arg2);
				
				
				PyObject *pValue = PyObject_CallObject(pFunc,args);
				
				
				const char *s=PyUnicode_AsUTF8(pValue);
				cout<<s<<endl;
				f->mvt_disp_p->update_img();
			}
			else cout << "Echec chargement fonction"<<endl;
		}
		else cout <<"Echec chargement script"<<endl;
	}
	else cout << "Veuillez selectionner 2 fichiers"<<endl;
}

void MvtPanel::liste_fichiers(wxArrayString p)
{
	wxString s;
	for(int i=0;i<p.GetCount();i++){
		s+=p.Item(i)+wxT("\n");
	}
	this->files_list->SetLabel(s);
}


bool MvtPanel::ShouldInheritColours()const{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(DispPanel, wxPanel)

EVT_PAINT(DispPanel::paintEvent)
EVT_SIZE(DispPanel::OnSize)

END_EVENT_TABLE()

DispPanel::DispPanel(wxPanel *parent,wxString p):wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
	parent_panel=parent;
	this->path=p;
	image.LoadFile(this->path, wxBITMAP_TYPE_JPEG );
	w = 1;
    h = 1;
	wxClientDC dc(this);
    render(dc);
}

void DispPanel::render(wxDC& dc){
	int neww, newh;
	dc.GetSize( &neww, &newh );
	if( neww != w || newh != h )
	{
		if(neww>410 or newh>308){neww=410;newh=308;}
		else{
			float ratio=min((float)neww/410.0,(float)newh/308.0);
			newh=308*ratio;
			neww=410*ratio;
		}
		if(newh>0 and neww>0){
			resized = wxBitmap(image.Scale(neww,newh,wxIMAGE_QUALITY_HIGH));
			w = neww;
			h = newh;
			dc.DrawBitmap( resized, 0, 0, false );
		}
	}
	else{
		dc.DrawBitmap( resized, 0, 0, false );
	}	
}

void DispPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
    //evt.skip();
}

void DispPanel::OnSize(wxSizeEvent& event){
    Refresh();
    event.Skip();
}

void DispPanel::update_img(){
	image.LoadFile(this->path, wxBITMAP_TYPE_JPEG );
	wxClientDC dc(this);
    render(dc);
}

