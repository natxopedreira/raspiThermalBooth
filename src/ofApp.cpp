#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_ERROR);
	ofSetVerticalSync(false);
	ofSetFrameRate(60);
	ofHideCursor();

	lastImageID = "";
	cuantasPendientes = 0;
	impresas = 0;
	hastagInstagram = "myhastagestrela";
	
	//// assets
	pie.loadImage("pie.jpg");
	cabecera.loadImage("cabecera.jpg");
	pieImpresionColor.loadImage("piePrint.png");

	//// fbo para la impresion termica
	instaPhotoWidth = 640;
	instaPhotoHeight = 640;

	fboTermica.allocate(instaPhotoWidth,cabecera.getHeight()+instaPhotoHeight+pie.getHeight());
	fboTermica.begin();
	ofClear(255);
	fboTermica.end();

	//// fbo para la impresion en color 10X15
	fboColorPrint.allocate(1076, 1600);
	fboColorPrint.begin();
	ofClear(255);
	fboColorPrint.end();
		
	newWidth = fboColorPrint.getWidth();
	newHeight = instaPhotoHeight/instaPhotoWidth*newWidth;

	/// timers para consultar en la bd, que hay nuevo y mandar a imprimir
    listImagesTimer.setup(8000);
	listImagesTimer.start(true);


	/// impresora termica
	printer.initThermalPrinter();
	printer.startThread();
	
	/// base de datos
	string ruta = ofToDataPath("/home/pi/example.sqlite", true);
	db = new SQLite::Database(ruta, SQLITE_OPEN_READWRITE);

	
	/// hack en la ventana
	ofSetWindowShape(1960,1100);
	ofSetWindowPosition(3,10);
	
    ofAddListener(listImagesTimer.TIMER_COMPLETE, this, &ofApp::timer1CompleteHandler);

	//ofAddListener(threadedInstagramData.newData, this, &ofApp::newDataInstagram);
	ofAddListener(printer.unaMenos, this, &ofApp::updateRecord);
	

	//fuente
	fuenteNombre.loadFont("HelveticaNeue-Medium.otf",25);
	fuenteMensaje.setup("Helvetica-Light.ttf",1.4);


	//ciconoes
	iconoInstagrm.loadImage("instaIcono.png");
	iconoTwitter.loadImage("twitter.png");

	//threaded texture
	//myTex = new ofTexture(); //create your own texture, it will be cleared so be sure its empty 
 
 
	ofAddListener(getImages.IMAGE_LOADED, this, &ofApp::textureReady);
 	//progressiveTextureLoader.setup(myTex, CV_INTER_CUBIC); 
	//progressiveTextureLoader.setVerbose(true); 
	//progressiveTextureLoader.setScanlinesPerLoop(512);//see header for explanation 
 	//progressiveTextureLoader.setTargetTimePerFrame(10.0f); //how long to spend uploading data per frame, in ms 

	listaTextura = true;

	std::cout << "setup()" << std::endl;
}

//--------------------------------------------------------------
void ofApp::update(){
     listImagesTimer.update();

     printer.updatePrinter();

	 //progressiveTextureLoader.update();
	/// thread de impresion
}

//--------------------------------------------------------------
void ofApp::draw(){
   /* ofBackground(0,255,0);
	//drawPhotos();
	if(listaTextura) drawPhotos();
*/
	ofDrawBitmapStringHighlight("framerate: " + ofToString(ofGetFrameRate()),100,100, ofColor(255,0,0),ofColor(255,255,255));
	ofDrawBitmapStringHighlight("fotos pendientes: " +ofToString(printer.q.size()),100,120, ofColor(255,0,0),ofColor(255,255,255));
	ofDrawBitmapStringHighlight("fotos impresas: " +ofToString(impresas),100,140, ofColor(255,0,0),ofColor(255,255,255));
	ofDrawBitmapStringHighlight("hastag instagram: " +hastagInstagram,100,160, ofColor(255,0,0),ofColor(255,255,255));
	
}
//--------------------------------------------------------------
void ofApp::drawPhotos(){
    ofPushMatrix();
	ofTranslate(25,920);
	ofScale(.4,.4);
	ofRotate(270);
            if (materialImprimir.imagen.isAllocated()) {
				fboTermica.draw(20,20);
				fboColorPrint.draw(70+fboTermica.getWidth(),20);
            }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::fillFbo(){


	newWidth = fboColorPrint.getWidth();



	if(materialImprimir.fuente == "instagram"){
		newHeight = instaPhotoHeight/instaPhotoWidth*newWidth;
	}else{
		if(materialImprimir.imagen.isAllocated()){
			newHeight = materialImprimir.imagen.getHeight()/materialImprimir.imagen.getWidth()*newWidth;
		}else{
			newHeight = instaPhotoHeight/instaPhotoWidth*newWidth;
		}
	}

	////////////////////////////////////////
	////////////////////////////////////////
	////////////////////////////////////////
	//// fbo para la impresion termica; ////
	////////////////////////////////////////
	////////////////////////////////////////
	fboTermica.begin();
		ofClear(255);
		ofRect(0,0, fboTermica.getWidth(), fboTermica.getHeight());

		// cabecera
		cabecera.draw(0,0);

		// insta photo si esta cargada
		
		//myTex->draw(0,cabecera.getHeight());
		if(socialPhoto.isAllocated())socialPhoto.draw(0,cabecera.getHeight());
		//if(materialImprimir.imagen.isAllocated()) materialImprimir.imagen.draw(0,cabecera.getHeight());

		// pie
		pie.draw(0, cabecera.getHeight() + instaPhotoHeight);

		// bidi si esta cargada la photo del instagram
		//if(materialImprimir.imagen.isAllocated()) qrCodeGenerator.generate(materialImprimir.url, 164, true, false).draw(52, cabecera.getHeight() + instaPhotoHeight + 390);
		qrCodeGenerator.generate("http://www.empathytool.com/estrella360/index.php?url=" + materialImprimir.url, 164, true, false).draw(52, cabecera.getHeight() + instaPhotoHeight + 390);

	fboTermica.end();







	////////////////////////////////////////
	////////////////////////////////////////
	////////////////////////////////////////
	//// fbo para la impresion en color ////
	////////////////////////////////////////
	////////////////////////////////////////
/*
	fboColorPrint.begin();
		ofClear(255);
		ofSetColor(255);

		//mancha blanca
		ofRect(0,0,fboColorPrint.getWidth(),fboColorPrint.getHeight());

		// foto instagram
		//if(materialImprimir.imagen.isAllocated()) materialImprimir.imagen.draw(0,0,newWidth,newHeight);
		myTex->draw(0,0,newWidth,newHeight);


		// pie estrella
		pieImpresionColor.draw(20, fboColorPrint.getHeight()-pieImpresionColor.getHeight());

		// texto
		if(materialImprimir.fuente == "instagram"){
			iconoInstagrm.draw(20, newHeight+10);
			ofSetColor(0);
			fuenteNombre.drawString(materialImprimir.user, 30 + iconoInstagrm.getWidth(), newHeight+35);
		}else{
			iconoTwitter.draw(20, newHeight+10);
			ofSetColor(0);
			fuenteNombre.drawString(materialImprimir.user, 30 + iconoTwitter.getWidth(), newHeight+35);
		}
		
		//fuenteMensaje.setText(materialImprimir.mensaje);
		int lineas = 0;
		fuenteMensaje.drawMultiLineColumn(materialImprimir.mensaje, 25,23, newHeight+70,1000,lineas,false,2);




	fboColorPrint.end();
	*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//imprimimos

		

}
//--------------------------------------------------------------
void ofApp::textureReady(string & str){
	
	
	if(ofGetElapsedTimeMillis()< 10000) return;
	std::cout  << "liiiisto" << std::endl;
	fillFbo();

	ofPixels pxsTermica;
	ofPixels pxsColor;
			


	fboTermica.readToPixels(pxsTermica);
	fboColorPrint.readToPixels(pxsColor);

	ofPixels pxs = imgUtil.dither(pxsTermica);
        

	printer.addFrame(pxs,pxsColor,lastImageID);

	listaTextura = true;
} 
//--------------------------------------------------------------
void ofApp::listImages(){
    // conectamos la primera vez y pillamos el ID de la ultima
    try{
		std::string instagramBD = ofToDataPath("/home/pi/example.sqlite", true);
		
		

		bdPhotoBooth consultaInsta(instagramBD);
		consultaInsta.getData();

		if(consultaInsta.getNumRows()>0){
			if(consultaInsta.getUrl() !="" && !printer.checkId(consultaInsta.getIdFoto())){
					materialImprimir.imagen.clear();
					materialImprimir.idFoto = consultaInsta.getIdFoto();
					materialImprimir.user = consultaInsta.getUser();
					materialImprimir.mensaje = consultaInsta.getTxt();
					materialImprimir.fuente = consultaInsta.getFuente();
					materialImprimir.url = consultaInsta.getUrl();

					

					if(listaTextura){
						find_and_replace(materialImprimir.url, "https", "http");
						
						std::cout << "loadFromURL " << materialImprimir.url << std::endl;
						
						getImages.loadFromURL(socialPhoto, materialImprimir.url);
						lastImageID =consultaInsta.getIdFoto();
						listaTextura = false;

					}
					//getImages.loadFromURL(materialImprimir.imagen, materialImprimir.url);

					/*
					
					if(!progressiveTextureLoader.isDoingWorkInThread()){
						find_and_replace(materialImprimir.url, "https", "http");
						//cout << "!progressiveTextureLoader.isDoingWorkInThread()" << materialImprimir.url << endl;

						listaTextura = false;
						myTex->clear(); 
 						progressiveTextureLoader.loadTexture(materialImprimir.url,true); 
						lastImageID =consultaInsta.getIdFoto();
					}else{
						//cout << "progressiveTextureLoader.isDoingWorkInThread()" << materialImprimir.url << endl;
					}

					*/
			}
		}

		
        }catch (exception ex){
		cout << ex.what() << endl;
	}
}
//--------------------------------------------------------------
void ofApp::updateRecord(string & _idFoto){
	try{
		db->exec("UPDATE impresiones SET impresa=1 WHERE idFoto='"+_idFoto+"'");

		// elimina de la lista de pendientes
		printer.quitaDeLaLista(_idFoto);

		impresas++;
		if(cuantasPendientes>0) cuantasPendientes--;

	}catch (exception ex){
		cout << ex.what() << endl;
	}
}

//--------------------------------------------------------------
void ofApp::find_and_replace(string& source, string const& find, string const& replace)
{
    for(string::size_type i = 0; (i = source.find(find, i)) != string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length();
    }
}
//--------------------------------------------------------------
void ofApp::timer1CompleteHandler( int &args )
{
    listImages();
	//cout <<  "listImages" << endl;
}

//--------------------------------------------------------------
void ofApp::newDataInstagram(string & s){
    listImages();
}

//--------------------------------------------------------------
void ofApp::exit()
{

	getImages.stopThread();
	printer.waitForThread();
	
}

//--------------------------------------------------------------
string ofApp::formatString(string format, int number) {

    char buffer[100];
    sprintf(buffer, format.c_str(), number);
    return (string)buffer;
}
