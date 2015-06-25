#pragma once

#include "ofMain.h"
#include "ofxThreadedImageLoader.h"
#include "ofxSimpleTimer.h"
#include "ofxImpresionThreaded.h"
#include "ofxQRCodeGenerator.h"
#include "SQLiteCpp.h"
//#include "vistaBrowser.h"
#include "ofxFontStash.h"
//#include "ofxProgressiveTextureLoad.h"
#include "bdPhotoBooth.h"
#include "utilidades.h"

typedef struct {   
    ofImage imagen;
	string user;
	string mensaje;
	string idFoto;
	string fuente;
	string url;
} contenidoImpresion;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);

		void drawPhotos();
		void fillFbo();
		void listImages();

		void newDataInstagram(string & s);
		void updateRecord(string & _idFoto);

		void timer1CompleteHandler( int &args );
		void timerImprimeFinish( int &args );

		void textureReady(string & str);

		void find_and_replace(string& source, string const& find, string const& replace);

		string formatString(string format, int number);


    ofxSimpleTimer listImagesTimer;

	ofTexture * myTex;
	//ofxProgressiveTextureLoad progressiveTextureLoader;
    ofxThreadedImageLoader getImages;
    ofImage socialPhoto;

	ofxImpresionThreaded printer;

	ofxQRCodeGenerator qrCodeGenerator; 

	SQLite::Database * db;

	bool listaTextura;

	utilidades imgUtil;
	//datosThread threadedInstagramData;

	ofImage pie,cabecera, pieImpresionColor;

	string hastagInstagram;
	string lastImageID;
    string urlFoto;

	bool fotoVisible;
	bool imprime;

	ofFbo fboTermica;
	ofFbo fboColorPrint;
	
	int instaPhotoWidth;
	int instaPhotoHeight;
	
	int cuantasPendientes;
	int impresas;

	int newWidth;
	int newHeight;

	ofTrueTypeFont fuenteNombre;
	ofxFontStash fuenteMensaje;

	ofImage iconoInstagrm;
	ofImage iconoTwitter;

	contenidoImpresion materialImprimir;
};
