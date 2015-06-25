/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"    
#include "ofxCUPS.h"
#include "LocalAddressGrabber.h"


typedef struct {   
    ofPixels imageThermalPrint;
	//ofPixels imageThermalPrintColor;
	string idFoto;
} QueuedImage;


class ofxImpresionThreaded : public ofThread {    
public:    
  
	//ofx::ESCPOS::DefaultSerialPrinter printer;
    int counter;  
    queue<QueuedImage> q;
	vector<string> impresasCurso;

	ofEvent<string> unaMenos;

	LocalAddressGrabber myIp;
//-----------------------------------	---------------------------

	ofxCUPS printer;

    ofxImpresionThreaded(){  
        counter=0;
    }  

//--------------------------------------------------------------

	void initThermalPrinter(){
// Get default printer
        string defaultPrinterName = printer.getDefaultPrinterName();
        cout << "Default Printer: " << defaultPrinterName << endl;
        
        
        // set printer name which you want to use...
        printer.setPrinterName(defaultPrinterName);
        printer.addOption("scaling","99");
		printer.setJobTitle("raspiPhotoBooth");

		// print ip
		//printStatus();
	}

	void updatePrinter(){
 		printer.updatePrinterInfo();
	}

	void printStatus(){
		string str = myIp.getIpAddress();
		ofBuffer msg(str.c_str(), str.length());
		ofBufferToFile("out.txt", msg);
		printer.printImage("outx.txt");
	}
//--------------------------------------------------------------
  
    void threadedFunction() {    
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
				string respuesta = i.idFoto;

				// guarda la imagen
				ofSaveImage(i.imageThermalPrint, ofToString(respuesta)+".png");

				string nome = ofToString(respuesta)+".png";
				printer.printImage(nome);

				std::cout << ofToString(respuesta)+".png" << std::endl;

				//impresion termica
				/*				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.printImage(i.imageThermalPrint,OF_ALIGN_HORZ_IGNORE,0.1,0.094,ofx::ESCPOS::BaseCodes::RESOLUTION_24_DOTS_DOUBLE_DENSITY);
                printer.cut(ofx::ESCPOS::BaseCodes::CUT_FULL);
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				printer.println(" ");
				//impresion color
				//ofSystem("rundll32.exe c:\\Windows\\System32\\shimgvw.dll, ImageView_PrintTo c:\\temp\\test.png \"DS-RX1 (Copiar 2)\""); 
				*/
				// avisa pa que la marquen como impresa
				ofNotifyEvent(unaMenos,respuesta,this);
				q.pop();
							
            }
        }
    }   

//--------------------------------------------------------------

        
	void addFrame(ofPixels imageToPrint,ofPixels imageToPrintColor,string ideFoto) {  
        
        QueuedImage qImage;
        qImage.imageThermalPrint = imageToPrint;
		//qImage.imageThermalPrintColor = imageToPrintColor;
		qImage.idFoto = ideFoto;

		q.push(qImage);
        
		/// agregas el id a la lista
		impresasCurso.push_back(ideFoto);

    }    

//--------------------------------------------------------------
	bool checkId(string _idBusca){
		/// comprueba si el valor del id de foto esta en la lista de impresasCurso
		bool response = false;
		
		for (int i = 0; i < impresasCurso.size(); i++)
		{
			if(impresasCurso.at(i) == _idBusca && !response) response = true;
		}

		return response;
	}
//--------------------------------------------------------------
	void quitaDeLaLista(string _idBusca){
		// borra de la lista de la cola pa imprimir
		for (int i = 0; i < impresasCurso.size(); i++)
		{
			if(impresasCurso.at(i) == _idBusca){
				impresasCurso.erase(impresasCurso.begin()+i);
			}
		}
	}

};  
