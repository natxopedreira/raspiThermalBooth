/*
busca en instagram y mira en bd
*/

// base de datos
var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database('example.sqlite');

// instagram
var Instagram = require('instagram-node-lib');
Instagram.set('client_id', '8d68a252a960412286633a59deacc960');
Instagram.set('client_secret', '89657a3aa3f541d6bb6fa9848347ac80');

//twitter
var Twit = require('twit')
var T = new Twit({
    consumer_key:         'QNyIC4e0eczFKxV0YCZuNDdBe'
  , consumer_secret:      'ylAqjw2RZeo6kA03moJhUktQwxLSQ8iFkqv4XjRYQOo6SfLgou'
  , access_token:         '3200731204-eXdc9B39SG4qoVndrgDB7sUdjf314bW6X2fs3Ji'
  , access_token_secret:  'QF15oywr0l7qee40cKtXbEVPyfh9iGMf3arh7D6737jV9'
})

var lasTwitId = "";

miraSiEstaInstagram = function(datos){
	// console.log(" -------------------------------- ");
	// console.log(datos.length);

	 for (var i = datos.length - 1; i >= 0; i--) {
	 	console.log(datos[i][4]);

	 	var idl = datos[i][0];
	 	var impresa = 0;
	 	var url = datos[i][1];
	 	var txt = datos[i][2];
	 	var user = datos[i][3];
	 	var fuente = datos[i][4];

	 	
	 	marron(idl,impresa,url,txt,user,fuente);

	}
	this.buscando = 0;
}

miraSiEstaTwitter = function(datosTweet){
	 //console.log(" -------------------------------- ");
	 //console.log(datosTweet.length);

	 for (var i = datosTweet.length - 1; i >= 0; i--) {
	 	//console.log(datosTweet[i][4]);
	 	
	 	var idl = datosTweet[i][0];
	 	var impresa = 0;
	 	var url = datosTweet[i][1];
	 	var txt = datosTweet[i][2];
	 	var user = datosTweet[i][3];
	 	var fuente = datosTweet[i][4];

	 	
	 	marron(idl,impresa,url,txt,user,fuente);

	}
	this.buscando = 0;
}

marron = function(idFoto,impresa,url,txt,user,fuente){
	db.get("SELECT COUNT(idFoto) AS idl FROM impresiones WHERE idFoto = '"+idFoto+"'", function (err, row) {
		if(row.idl == 0){
			db.run("INSERT INTO impresiones(idFoto,impresa,url,txt,user,fuente) VALUES ($idFoto,$impresa,$url,$txt,$user,$fuente)", {
				$idFoto:idFoto,
		        $impresa: impresa,
		        $url: url,
		        $txt: txt,
		        $user: user,
		        $fuente: fuente
		    });
		}
	})
}


// Public
module.exports = Busca;


function Busca(){
	this.datos = [];
	this.datosTweet = [];
	this.buscando = 0;
}

Busca.prototype.ocupado = function(){
	return this.buscando;
}


Busca.prototype.mirainstagram = function(hashtag,cuantos){
	Instagram.tags.recent({
  		name: hashtag,
  		count: cuantos,
  		complete: function(data, pagination){
	        //console.log(data.length);
	        this.datos = [];
	        var cantidad = 0;
	        for (var i = 0; i < data.length; i++) {
	        	
	        	var extension = data[i].images.standard_resolution.url.split('.').pop();
          		
          		if(extension == "jpg"){
          			
          			datos[cantidad] = [data[i].caption.id, data[i].images.standard_resolution.url, data[i].caption.text, data[i].caption.from.username,"instagram"];
            		cantidad++;
          		}
	        }
	        miraSiEstaInstagram(this.datos);
	        this.buscando = 1;
	        //console.log(datos.length);
      },
      error: function(errorMessage, errorObject, caller){
      // errorMessage is the raised error message
      // errorObject is either the object that caused the issue, or the nearest neighbor
      // caller is the method in which the error occurred
      }

  })
}

Busca.prototype.miraTwitter = function(hashtag){
	T.get('search/tweets', { q: '#'+hashtag, include_entities:true}, function(err, data, response) {
      if(err){
        console.log("twitter search error");
        console.log(err);
      }

      if(data != null){
      
      this.datosTweet = [];
      var cantidad = 0;

      for (var i = 0; i < data.statuses.length; i++) {
        
        if(data.statuses[i].entities.media[0].media_url != null){
          
          var extension = data.statuses[i].entities.media[0].media_url.split('.').pop();
          if(extension == "jpg"){

              	datosTweet[cantidad] = [data.statuses[i].entities.media[0].id.toString(), data.statuses[i].entities.media[0].media_url, data.statuses[i].entities.hashtags[0].text, data.statuses[i].user.name,"twitter"];
            	cantidad++;
          }
        }

      }
      miraSiEstaTwitter(this.datosTweet);
    }
  })
}






// sudo node -expose-gc demoLeak.js

var buscador = new Busca();

setInterval(function() {
  buscador.mirainstagram('myhastagestrela',20);
  buscador.miraTwitter('myhastagestrela');
  global.gc();
}, 10000);

