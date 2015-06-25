#include "SQLiteCpp.h"
#include <string>

class bdPhotoBooth{
public:
	bdPhotoBooth(const std::string& dbFile):mDb(dbFile),mQuery(mDb, "SELECT idFoto, impresa, url, txt, user, fuente from impresiones WHERE impresa='0' LIMIT 1"){
		numRows = 0;
		_idFoto = "";
		_impresa = 0;
		_url = "";
		_txt = "";
		_user = "";
	}

	void getData(){
		_idFoto = "";
		numRows = 0;

		while (mQuery.executeStep())
		{
			_idFoto = mQuery.getColumn(0).getText();
			_impresa = mQuery.getColumn(1).getInt();
			_url = mQuery.getColumn(2).getText();
			_txt = mQuery.getColumn(3).getText();
			_user = mQuery.getColumn(4).getText();
			_fuente = mQuery.getColumn(5).getText();
			numRows++;
		}
	}
	

	std::string getIdFoto(){
		return _idFoto;
	}

	int getImpresa(){
		return _impresa;
	}

	std::string getUrl(){
		return _url;
	}

	std::string getTxt(){
		return _txt;
	}

	std::string getUser(){
		return _user;
	}

	std::string getFuente(){
		return _fuente;
	}
	
	int getNumRows(){
		return numRows;
	}


private:
	SQLite::Database    mDb;    ///< Database connection 
	SQLite::Statement   mQuery; ///< Database prepared SQL query 
	std::string _idFoto;
	int _impresa;
	std::string _url;
	std::string _txt;
	std::string _user;
	std::string _fuente;
	int numRows;
};
