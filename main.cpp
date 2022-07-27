#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <windows.h>

using namespace std;
//---------Globalne varijable-------//
int brojFilmova = 0;
int brojSala = 0;
const int MAX_SALA = 8;
// -----------Enumeracija------------//
enum izbor {
	NESPREMAJ, SPREMAJ
};

enum zanrovi {
	KRIMI_FILM = 1, 
	HISTORIJSKI_FILM,	
	CRNI_FILM,
	TINEJDZERSKI_FILM,
	SPORTSKI_FILM,
	NAUCNO_FANTASTICKI_FILM,
	RATNI_FILM,
	VESTERN_FILM,
	AKCIJA,
	SUPER_HEROJSKI,
	AVANTURA,
	KOMEDIJA,
	DRAMA,
	FANTAZIJA,
	HOROR,
	MISTERIJA,
	ROMANTIKA,
	TRILER,
	KLASICNI,
	ANIMACIJA,
	DJECIJI_FILM,
	BIOGRAFSKI,
	MJUZIKL,
	DOKUMENTARNI,
	PORODICNI,
	FILM_ZA_ODRASLE
};
//-------------Strukture-------------//
struct film {
	char naziv_filma[20],
		 zanr[25],
		 format[3],
		 datum_izlaska[11], 
		 glavni_glumac[20],
		 reziser[20],
		 mjesto_radnje[20],
		 direktor[20],
		 jezik[20];
	int trajanje_filma;
};

struct sala {
	film film;
	char show_time[8];
	int broj_sale,
		broj_sjedista = 100,
		slobodna_sjedista;
	float cijena;
};
//-------------Funkcije-------------//
void korisnicki_meni();
void administratorski_meni();
void kupi_kartu();
void dodaj_novi_film();
void unos_charactera(char *, int);
void uredi_film(char *);
void izbrisi_film(char *);
void prikazi_filmove();
void ucitajFilmove(film *);
void upisiFilmove(film *);
void prebrojiFilmove();
void prebrojiSale();
void UbaciUSalu();
void UpisiSalu(sala, int);
void PrebrojiSale();
void UcitajSjedista(int, int**);
void IzmjeniSalu(int, int **);
void UcitajSale(sala *);
void UpisiSale(sala *);
void ResetujSale();
void PrikaziSalu(sala *);
void loading(const char *);

int korisnicki_izbor();
int administratorski_izbor();
int uredjivanjeFilmaMeni();
int suma(int, int);

bool provjera(string, string);
bool provjeraShowTime(char *);

sala *ucitaj_salu(int);
//----------------------------------//
int main() {
	prebrojiSale();
	prebrojiFilmove();
	korisnicki_meni();
	return 0;
}

void korisnicki_meni() {

	int izbor;
	while(izbor = korisnicki_izbor()) {
		switch(izbor) {
			case 1: {
				if(brojSala == 0) {
					cout << "\n\t\t\t\t  Trenutno nemamo filmova na raspolaganju.\n";
					cout << "\t\t\t\t  Dodjite nam opet, s Vama Kino Oscar.";
					Sleep(10000);
				} else kupi_kartu();
				break;
			}
			case 2: {
				if(brojSala == 0) {
					cout << "\n\t\t\t\t  Trenutno nemamo filmova na raspolaganju.\n";
					cout << "\t\t\t\t  Dodjite nam opet, s Vama Kino Oscar.";
					Sleep(10000);
				} else {
					sala *sale = new sala[brojSala];
					UcitajSale(sale);
					for(int i = 0; i < brojSala; i++) {
						cout << "\t\t\t\t  " << i+1 << ". " << sale[i].film.naziv_filma << endl;
					}
					cout << "\t\t\t\t  Unesi redni broj filma: ";
					int n;
					do {
						cin >> n;
						if(n < 1 || n > brojSala) {
							cout << "\t\t\t\t  Broj filma mora biti u rasponu od 1-" << brojSala << endl;
							cout << "\t\t\t\t  Ponovo unesi broj filma: ";
						}
					} while(n < 1 || n > brojSala);
					system("cls");
					PrikaziSalu(ucitaj_salu(n-1));
					delete[] sale;
					cout << "\n\t\t\t\t  Pritisnite enter za nastavak...";
					getch();					
				}
				break;
			}
			case 3: {
				administratorski_meni();
				break;
			}
			default: {
				return;
				break;
			}
		}
	}	
}

void administratorski_meni() {
	
	system("cls");
	string username, sifra;
	int pokusaji = 5;
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t   _     ___    ___  ___  _  _ \n";
	cout << "\t\t\t\t  | |   / _ \\  / __||_ _|| \\| |\n";
	cout << "\t\t\t\t  | |__| (_) || (_ | | | | .` |\n";
	cout << "\t\t\t\t  |____|\\___/  \\___||___||_|\\_|\n\n";
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	while(pokusaji > 0) {
		cout << "\t\t\t\t  Unesite username: ";
		cin >> username; 
		cin.ignore();
		cout << "\t\t\t\t  Unesite sifru: ";
		sifra.clear();
		char znak;
		while (znak = _getch()) {
		    if (znak != 13) cout << "*";
		    sifra += znak;
		    if (znak == 13) break;
		}
		sifra.erase(sifra.length() - 1);
		cout << endl;
		if(provjera(username, sifra)) break;
		else pokusaji--;			
		if(pokusaji == 0) {
			cout << "\n\t\t\t\t  Previse pokusaja login-a.";
			cout << "\n\t\t\t\t  Izlazak iz sistema";
			for(int i = 0; i < 3; i++) {
				cout << ".";
				Sleep(1000);
			}
			exit(0);
		}
	}
	if(provjera(username, sifra)) {
		cout << endl;
		loading(" Ucitavanje Administratorskih postavki:");
		system("cls");
		int izbor;
		while(izbor = administratorski_izbor()) {
			switch(izbor) {
				case 1: {
					dodaj_novi_film();
					cout << "\t\t\t\t  Pritisnite enter za nastavak...";
					getch();
					system("cls");
					break;
				}
				case 2: {
					cin.ignore();
					cout << endl;
					char ime[20];
					cout << "\t\t\t\t  Unesi naziv filma: ";
					cin.getline(ime, 20);
					if(strlen(ime) == 0) {
						cout << "\t\t\t\t  Niste unijeli ime." << endl;
						cout << "\t\t\t\t  Sistem vas vraca u Admin meni." << endl;
					}
					else uredi_film(ime);
					cout << "\n\t\t\t\t  Pritisnike enter za nastavak...";
					getch();
					system("cls");
					break;
				}
				case 3: {
					if(brojFilmova == 0) {
						cout << "\t\t\t\t  Trenutno nema filmova u Bazi podataka.";
						cout << "\n\t\t\t\t  Pritisnite enter za nastavak...";
						getch();
					} else {
						system("cls");
						cin.ignore();
						cout << "\t\t\t\t--------------------------------------------\n";
						cout << "\t\t\t\t  ___       _                   _      \n";
						cout << "\t\t\t\t | _ ) _ _ (_) ___ __ _  _ _   (_) ___ \n";
						cout << "\t\t\t\t | _ \\| '_|| |(_-</ _` || ' \\  | |/ -_)\n";
						cout << "\t\t\t\t |___/|_|  |_|/__/\\__,_||_||_|_/ |\\___|\n";
						cout << "\t\t\t\t                             |__/      \n";
						cout << "\t\t\t\t                 __  _  _              \n";
						cout << "\t\t\t\t                / _|(_)| | _ __   __ _ \n";
						cout << "\t\t\t\t               |  _|| || || '  \\ / _` |\n";
						cout << "\t\t\t\t               |_|  |_||_||_|_|_|\\__,_|\n";
						cout << "\t\t\t\t--------------------------------------------\n";
						char ime[20];
						cout << "\t\t\t\t Unesi naziv filma: ";
						cin.getline(ime, 20);
						cout << endl;				
						if(strlen(ime) == 0) cout << "\t\t\t\t Niste unijeli ime." << endl;
						else {
							loading(" Brisanje filma u toku...");
							cout << endl;
							izbrisi_film(ime);
						}
						cout << "\t\t\t\t Pritisnike enter za nastavak...";
						getch();
						system("cls");						
					}
					break;
				}
				case 4: {
					system("cls");
					cout << "\t\t\t\t ----------------------------------------------------------\n";
					cout << "\t\t\t\t   ___  _  _                   _ \n";
					cout << "\t\t\t\t  | __|(_)| | _ __   ___ __ __(_)\n";
					cout << "\t\t\t\t  | _| | || || '  \\ / _ \\\\ V /| |\n";
					cout << "\t\t\t\t  |_|  |_||_||_|_|_|\\___/ \\_/ |_|\n\n";
					prikazi_filmove();
					cout << "\n\t\t\t\t Pritisnite enter za nastavak...";
					getch();
					break;
				}
				case 5: {
					UbaciUSalu();
					break;
				}
				case 6: {
					system("cls");
					cout << endl;
					ResetujSale();
					loading(" Resetovanje sali u toku...");
					cout << "\n\n\t\t\t\t  Sale resetovane uspjesno.";
					cout << "\n\t\t\t\t  Pritisnite enter za nastavak...";
					getch();
					break;
				}
				default: {
					return;
					break;
				}
			}	
		}		
	}
}

void kupi_kartu() {
	
	system("cls");
	sala *Sale = new sala[brojSala];
	UcitajSale(Sale);
	cout << "\t\t\t\t-----------------------------------\n";
	cout << "\t\t\t\t Spisak filmova:\n";
	for(int i = 0; i < brojSala; i++) cout << "\n\t\t\t\t " << i+1 << ". " << Sale[i].film.naziv_filma;
	cout << "\n\t\t\t\t-----------------------------------\n";	
	cout << "\t\t\t\t Unesite ID filma: ";
	int redniBroj, pokusaji = 0;
	do {
		cin >> redniBroj;	
		if(redniBroj < 1 || redniBroj > brojSala) {
			cout << "\t\t\t\t Greska - Redni broj filmova mora biti u rasponu od 1-" << brojSala << ".\n";
			if(pokusaji == 5) {
				cout << "\t\t\t\t Previse pokusaja!\n";
				cout << "\t\t\t\t Sistem se vraca u glavni program.";
				main();
			}
			cout << "\t\t\t\t Ponovo unesite broj filma: ";
		}
		pokusaji++;
	} while(redniBroj < 1 || redniBroj > brojSala);
	cout << "\t\t\t\t Cijena karte: " << Sale[redniBroj-1].cijena << " KM."<< endl;
	int brojKarti;
	cout << "\t\t\t\t Unesite broj karti koje zelite kupiti: ";
	do {
		cin >> brojKarti;
		if(brojKarti == 0) {
			cout << "\t\t\t\t Odustali ste od kupovine karti." << endl;
			cout << "\t\t\t\t Pritisnite enter za nastavak...";
			getch();
			return;
		} else if(brojKarti < 0) {
			cout << "\t\t\t\t Ne mozete uzeto negativan broj karti." << endl;
			cout << "\t\t\t\t Ponovo unesite koliko karti zelite kupiti: ";
		} else if(brojKarti > Sale[redniBroj-1].slobodna_sjedista) {
			cout << "\t\t\t\t Ne mozete uzeti toliko karti jer nema " << endl
				 << "\t\t\t\t slobodnih mjesta u sali.";
			cout << "\t\t\t\t Ponovo unesite broj karti: ";
		}
	} while(brojKarti < 1 || brojKarti > Sale[redniBroj-1].slobodna_sjedista);
	cout << "\t\t\t\t Ukupna cijena karti: " << suma(Sale[redniBroj-1].cijena, brojKarti) << endl;
	cout << "\t\t\t\t Pritisnite enter za nastavak...";
	getch();
	system("cls");
	int **matrica = new int *[10];
	for(int i = 0; i < 10; i++) matrica[i] = new int[10];
	UcitajSjedista(redniBroj, matrica);
	cout << "\t\t\t\t-----------------------------------";	
	cout << "\n\t\t\t\t    Sala broj " << redniBroj << ".";
	cout << "\n\t\t\t\t-----------------------------------";	
	cout << "\n\t\t\t\t ";
	for(int i = 0; i < 10; i++) {
		cout << "  ";
		for(int j = 0; j < 10; j++) {
			if(matrica[i][j] == 0) {
				cout << "-" << "-" << " ";
				continue;
			}
			if(matrica[i][j] < 10) cout << 0;
			cout << matrica[i][j] << " ";
		}
		cout << endl << "\t\t\t\t ";
	}
	cout << "\n\t\t\t\t-----------------------------------";	
	int brojSjedista;
	bool imalSjedista = false;
	for(int i = 0; i < 10; i++) {
		for(int j = 0; j < 10; j++) {
			if(matrica[i][j] != 0) imalSjedista = true;
		}
	}
	if(!imalSjedista) {
		cout << "\n\t\t\t\t Sva sjedista su popunjena, molimo dodjite sljedeci put." << endl;
		cout << "\t\t\t\t Pritisnite enter za nastavak...";
		getch();
		return;
	} else {
		for(int k = 0; k < brojKarti; k++) {
			if(k > 0) {
				system("cls");
				cout << "\t\t\t\t-----------------------------------";	
				cout << "\n\t\t\t\t    Sala broj " << redniBroj << ".";
				cout << "\n\t\t\t\t-----------------------------------";	
				cout << "\n\t\t\t\t ";
				for(int i = 0; i < 10; i++) {
					cout << "  ";
					for(int j = 0; j < 10; j++) {
						if(matrica[i][j] == 0) {
							cout << "-" << "-" << " ";
							continue;
						}
						if(matrica[i][j] < 10) cout << 0;
						cout << matrica[i][j] << " ";
					}
					cout << endl << "\t\t\t\t ";
				}
				cout << "\n\t\t\t\t-----------------------------------";	
			}
			imalSjedista = false;
			do {
				cout << "\n\t\t\t\t Unesi broj sjedista (1-100): ";
				cin >> brojSjedista;
				if(brojSjedista < 1 || brojSjedista > 100) {
					cout << "\n\t\t\t\t Morate unijeti broj sjedista od 1 do 100.";
					continue;
				}	
				
				for(int i = 0; i < 10; i++) {
					for(int j = 0; j < 10; j++) {
						if(brojSjedista == matrica[i][j]) {
							matrica[i][j] = 0;
							Sale[redniBroj-1].slobodna_sjedista--;
							imalSjedista = true;
						}
					}
				}
				if(!imalSjedista) {
					cout << "\t\t\t\t To sjediste je zauzeto." << endl;
					cout << "\t\t\t\t Morate ponovo unijeti broj sjedista.";
				}		
			}while((brojSjedista < 1 || brojSjedista > 100) || imalSjedista == false);
			if(k == brojKarti-1) {
				system("cls");
				cout << "\t\t\t\t-----------------------------------";	
				cout << "\n\t\t\t\t    Sala broj " << redniBroj << ".";
				cout << "\n\t\t\t\t-----------------------------------";	
				cout << "\n\t\t\t\t ";
				for(int i = 0; i < 10; i++) {
					cout << "  ";
					for(int j = 0; j < 10; j++) {
						if(matrica[i][j] == 0) {
							cout << "-" << "-" << " ";
							continue;
						}
						if(matrica[i][j] < 10) cout << 0;
						cout << matrica[i][j] << " ";
					}
					cout << endl << "\t\t\t\t ";
				}
				cout << "\n\t\t\t\t-----------------------------------" << endl;	
				cout << "\t\t\t\t Karte su kupljene." << endl;
				cout << "\t\t\t\t Pritisnite enter za nastavak...";
				getch();
			}
		}	
	}
	IzmjeniSalu(redniBroj, matrica);
	UpisiSale(Sale);
	cin.ignore();
	delete[] Sale;
	for(int i = 0; i < 10; i++) delete[] matrica[i];
	delete[] matrica;
}

void dodaj_novi_film() {
	
	system("cls");
	cin.ignore();
	
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t      ___           _                          _      \n";
	cout << "\t\t\t\t     |   \\  ___  __| | __ _ __ __ __ _  _ _   (_) ___ \n";
	cout << "\t\t\t\t     | |) |/ _ \\/ _` |/ _` |\\ V // _` || ' \\  | |/ -_)\n";
	cout << "\t\t\t\t     |___/ \\___/\\__,_|\\__,_| \\_/ \\__,_||_||_|_/ |\\___|\n";
	cout << "\t\t\t\t                                            |__/      \n";
	cout << "\t\t\t\t                                __  _  _              \n";
	cout << "\t\t\t\t                               / _|(_)| | _ __   __ _ \n";
	cout << "\t\t\t\t                              |  _|| || || '  \\ / _` |\n";
	cout << "\t\t\t\t                              |_|  |_||_||_|_|_|\\__,_|\n";
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	ofstream datoteka;
	datoteka.open("Baza podataka/Filmovi.txt", ios::app);
	
	if(datoteka.fail()) {
		cout << "\t\t\t\t  Greska - Nije moguce otvoriti datoteku.";
		return;
	} else {
		
		film Film;
		cin.clear();
		cout << "\t\t\t\t  Unesite naziv filma: ";
		do {
			cin.getline(Film.naziv_filma, 20);
			if(strlen(Film.naziv_filma) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite naziv filma: ";
			}
		} while(strlen(Film.naziv_filma) == 0);
		cout << "\t\t\t\t  Zanrovi filma: ";
		string lista[26] = {
			"Krimi", "Historijski", "Crni", "Tinejdzerski", "Sportski", "Naucnofantasticki", "Ratni",
			"Vestern", "Akcija", "Superherojski", "Avantura", "Komedija", "Drama", "Fantazija", "Horor", "Misterija", "Romantika", "Triler",
			"Klasicni", "Animacija", "Djeciji", "Biografski", "Mjuzikl", "Dokumentarni", "Porodicni", "Film za odrasle" 
		};
		cout << endl;
		int br = 0;
		cout<< "\t\t\t\t  " ;
		for(int i = 0; i < 26; i++) 
		{
			if(br==2){cout<<"\n\t\t\t\t  "; br=0;}
			cout << i+1 << ". " << lista[i] << "\t\t";
			br++;
		}
		cout << "\n\t\t\t\t  Unesite opciju: ";
		int odabir;
		do {
			cin >> odabir;
			if(odabir < 1 || odabir > 26) {
				cout << "\t\t\t\t  Pogresan odabir." << endl;
				cout << "\t\t\t\t  Ponovo unesite odabir: ";
			}
		} while(odabir < 1 || odabir > 26);
		for(int i = KRIMI_FILM; i < FILM_ZA_ODRASLE; i++) {
			if(odabir-1 == i) strcpy(Film.zanr, lista[i].c_str());
		}
		cin.ignore();
		cout << "\t\t\t\t  Unesite format filma (2D, 3D): ";
		do {
			cin.getline(Film.format, 3);
			if(strcmp(Film.format, "2d") == 0) strcpy(Film.format, "2D");
			if(strcmp(Film.format, "3d") == 0) strcpy(Film.format, "3D");
			if((strcmp(Film.format, "2D") == 0) || (strcmp(Film.format, "3D") == 0)) break;
			else {
				cout << "\t\t\t\t  Neispravan format filma." << endl;
				cout << "\t\t\t\t  Unesite ponovo format filma: ";
			}
		} while(strlen(Film.format) == 0 || strcmp(Film.format, "2D") != 0 || strcmp(Film.format, "3D") != 0);
		cout << "\t\t\t\t  Unesite datum izlaska filma: ";
		do {
			cin.getline(Film.datum_izlaska, 11);
			if(strlen(Film.datum_izlaska) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite datum izlaska filma: ";
			}
		} while(strlen(Film.datum_izlaska) == 0);
		cout << "\t\t\t\t  Unesite ime glavnog glumca: ";
		do {
			cin.getline(Film.glavni_glumac, 20);
			if(strlen(Film.glavni_glumac) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite glavnog glumca: ";
			}
		} while(strlen(Film.glavni_glumac) == 0);	
		cout << "\t\t\t\t  Unesite ime rezisera: ";
		do {
			cin.getline(Film.reziser, 20);
			if(strlen(Film.reziser) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite ime rezisera: ";
			}
		} while(strlen(Film.reziser) == 0);	
		cout << "\t\t\t\t  Unesite mjesto radnje filma: ";
		do {
			cin.getline(Film.mjesto_radnje, 20);
			if(strlen(Film.mjesto_radnje) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite mjesto radnje filma: ";
			}
		} while(strlen(Film.mjesto_radnje) == 0);
		cout << "\t\t\t\t  Unesite ime direktora: ";
		do {
			cin.getline(Film.direktor, 20);
			if(strlen(Film.direktor) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite direktora: ";
			}
		} while(strlen(Film.direktor) == 0);	
		cout << "\t\t\t\t  Unesite jezik filma: ";
		do {
			cin.getline(Film.jezik, 20);
			if(strlen(Film.jezik) == 0) {
				cout << "\t\t\t\t  Morate popuniti podatke." << endl;
				cout << "\t\t\t\t  Ponovo unesite jezik filma: ";
			}
		} while(strlen(Film.jezik) == 0);			
		cout << "\t\t\t\t  Unesite vrijeme trajanja filma: ";
		do {
			cin >> Film.trajanje_filma;
			if(Film.trajanje_filma < 15) {
				cout << "\t\t\t\t  Film je prekratak.";
				cout << "\n\t\t\t\t  Unesite ponovo trajanje filma: ";
			} else if(Film.trajanje_filma > 1000) {
				cout << "\t\t\t\t  Film je predug.";
				cout << "\n\t\t\t\t  Unesite ponovo trajanje filma: ";
			}
		} while(Film.trajanje_filma < 15);
		datoteka << Film.naziv_filma << endl;
		datoteka << Film.zanr << endl;
		datoteka << Film.format << endl;
		datoteka << Film.datum_izlaska << endl;
		datoteka << Film.glavni_glumac << endl;
		datoteka << Film.reziser << endl;
		datoteka << Film.mjesto_radnje << endl;
		datoteka << Film.direktor << endl;
		datoteka << Film.jezik << endl;
		datoteka << Film.trajanje_filma << endl;
		prebrojiFilmove();
		datoteka.close();
	}	
}

void uredi_film(char *imeFilma) {		

	film *Filmovi = new film[brojFilmova];
	ucitajFilmove(Filmovi);

	int br = 0;
	bool isSaved = false;
	for(int i = 0; i < brojFilmova; i++) {
		if(strcmp(imeFilma, Filmovi[i].naziv_filma) == 0) {
			int izbor;
			while(izbor = uredjivanjeFilmaMeni()) {
				switch(izbor) {
					case 1: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novi naziv filma: ";
						do {
							cin.getline(Filmovi[i].naziv_filma, 20);
							if(strlen(Filmovi[i].naziv_filma) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].naziv_filma) == 0);
						isSaved = false;	
						break;
					}
					case 2: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite zanr filma: ";
						string lista[26] = {
							"Krimi", "Historijski", "Crni", "Tinejdzerski", "Sportski", "Naucnofantasticki", "Ratni",
							"Vestern", "Akcija", "Superherojski", "Avantura", "Komedija", "Drama", "Fantazija", "Horor", "Misterija", "Romantika", "Triler",
							"Klasicni", "Animacija", "Djeciji", "Biografski", "Mjuzikl", "Dokumentarni", "Porodicni", "Film za odrasle" 
						};
						cout << endl;
						int br = 0;
						cout<< "\t\t\t\t  " ;
						for(int i = 0; i < 26; i++) 
						{
							if(br==2){cout<<"\n\t\t\t\t  "; br=0;}
							cout << i+1 << ". " << lista[i] << "\t\t";
							br++;
						}
						cout << "\n\t\t\t\t  Unesite opciju: ";
						int odabir;
						do {
							cin >> odabir;
							if(odabir < 1 || odabir > 26) {
								cout << "\t\t\t\t  Pogresan odabir." << endl;
								cout << "\t\t\t\t  Ponovo unesite odabir: ";
							}
						} while(odabir < 1 || odabir > 26);
						for(int j = KRIMI_FILM; j < FILM_ZA_ODRASLE; j++) {
							if(odabir-1 == j) strcpy(Filmovi[i].zanr, lista[j].c_str());
						}
						cin.ignore();	
						isSaved = false;
						break;
					}
					case 3: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novi format filma: ";
						do {
							cin.getline(Filmovi[i].format, 3);
							if(strcmp(Filmovi[i].format, "2d") == 0) strcpy(Filmovi[i].format, "2D");
							if(strcmp(Filmovi[i].format, "3d") == 0) strcpy(Filmovi[i].format, "3D");
							if((strcmp(Filmovi[i].format, "2D") == 0) || (strcmp(Filmovi[i].format, "3D") == 0)) break;
							else {
								cout << "\t\t\t\t  Neispravan format filma." << endl;
								cout << "\t\t\t\t  Unesite ponovo format filma: ";
							}
						} while(strlen(Filmovi[i].format) == 0 || strcmp(Filmovi[i].format, "2D") != 0 || strcmp(Filmovi[i].format, "3D") != 0);
						isSaved = false;
						break;
					}
					case 4: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novi datum izlaska filma: ";
						do {
							cin.getline(Filmovi[i].datum_izlaska, 11);
							if(strlen(Filmovi[i].datum_izlaska) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].datum_izlaska) == 0);
						isSaved = false;
						break;
					}
					case 5: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novo trajanje filma: ";
						do {
							cin >> Filmovi[i].trajanje_filma;
							if(Filmovi[i].trajanje_filma < 15) {
								cout << "\t\t\t\t  Film je prekratak.";
								cout << "\n\t\t\t\t  Unesite ponovo trajanje filma: ";
							} else if(Filmovi[i].trajanje_filma > 1000) {
								cout << "\t\t\t\t  Film je predug.";
								cout << "\n\t\t\t\t  Unesite ponovo trajanje filma: ";
							}
						} while(Filmovi[i].trajanje_filma < 15);
						isSaved = false;
						break;
					}
					case 6: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novog glavnog glumca: ";
						do {
							cin.getline(Filmovi[i].glavni_glumac, 20);
							if(strlen(Filmovi[i].glavni_glumac) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].glavni_glumac) == 0);	
						isSaved = false;				
						break;
					}
					case 7: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novog rezisera filma: ";
						do {
							cin.getline(Filmovi[i].reziser, 20);
							if(strlen(Filmovi[i].reziser) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].reziser) == 0);	
						isSaved = false;						
						break;
					}
					case 8: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novo mjesto radnje filma: ";
						do {
							cin.getline(Filmovi[i].mjesto_radnje, 20);
							if(strlen(Filmovi[i].mjesto_radnje) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].mjesto_radnje) == 0);	
						isSaved = false;						
						break;
					}
					case 9: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novog direktora: ";
						do {
							cin.getline(Filmovi[i].direktor, 20);
							if(strlen(Filmovi[i].direktor) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].direktor) == 0);	
						isSaved = false;						
						break;
					}
					case 10: {
						cout << endl;
						cin.ignore();
						cout << "\t\t\t\t  Unesite novi jezik filma: ";
						do {
							cin.getline(Filmovi[i].jezik, 20);
							if(strlen(Filmovi[i].jezik) == 0) {
								cout << "\t\t\t\t  Morate popuniti podatke." << endl;
								cout << "\t\t\t\t  Ponovo unesite: ";
							}
						} while(strlen(Filmovi[i].jezik) == 0);	
						isSaved = false;						
						break;
					}
					case 11: {
						cout << "\n\t\t\t\t  Promjene uspjesno spremljene." << endl;
						cout << "\t\t\t\t  Pritisnite enter za nastavak...";
						getch();
						upisiFilmove(Filmovi);
						isSaved = true;
						break;
					}
					default: {
						if(!isSaved) {
							int odabir;
							cout << "\t\t\t\t  Niste spremili izmjene!";
							cout << "\n\t\t\t\t  Da li zelite spremiti ? (Da (1) ili Ne(0)";
							cout << "\n\t\t\t\t  Unesite opciju: ";
							do {
								cin >> odabir;
								if(odabir == SPREMAJ ^ odabir == NESPREMAJ) {
									break;
								}
								else{
									cout << "\t\t\t\t  Pogresan unos." << endl;
									cout << "\t\t\t\t  Ponovo unesite odabir: ";
								}
							}while(odabir != 1 ^ odabir != 0);
							if(odabir == SPREMAJ) {
								cout << "\n\t\t\t\t  Promjene uspjesno spremljene." << endl;
								cout << "\t\t\t\t   Pritisnite enter za nastavak...";
								getch();
								upisiFilmove(&Filmovi[0]);
							}
						}
						return;
						break;
					}
				}
			}
		} else {
			br++;
		}
		if(i == brojFilmova-1 && i == br) {
			cout << "\t\t\t\t Film nije pronadjen u sistemu.";
			return;
		}
	}	
	delete[] Filmovi;
}

void izbrisi_film(char *imeFilma) {
	
	film *novaLista = new film[brojFilmova-1];
	film *filmovi = new film[brojFilmova];
	
	ucitajFilmove(filmovi);
	
	int j = 0;
	
	for(int i = 0; i < brojFilmova; i++) {
		if(strcmp(imeFilma, filmovi[i].naziv_filma) == 0) continue;
		else {
			strcpy(novaLista[j].naziv_filma, filmovi[i].naziv_filma);
			strcpy(novaLista[j].zanr, filmovi[i].zanr);
			strcpy(novaLista[j].format, filmovi[i].format);
			strcpy(novaLista[j].datum_izlaska, filmovi[i].datum_izlaska);
			strcpy(novaLista[j].glavni_glumac, filmovi[i].glavni_glumac);
			strcpy(novaLista[j].reziser, filmovi[i].reziser);
			strcpy(novaLista[j].mjesto_radnje, filmovi[i].mjesto_radnje);
			strcpy(novaLista[j].direktor, filmovi[i].direktor);
			strcpy(novaLista[j].jezik, filmovi[i].jezik);
			novaLista[j].trajanje_filma = filmovi[i].trajanje_filma;
			j++;
		}
	}
	if(j == brojFilmova) cout << "\n\t\t\t\t Ne postoji film u sistemu.\n";
	else {
		cout << "\n\t\t\t\t Film uspjesno izbrisan.\n";
		brojFilmova = j;
	}
	upisiFilmove(novaLista);
	delete[] novaLista;
	delete[] filmovi;
}

void prikazi_filmove() {
	if(brojFilmova == 0) {
		cout << "\t\t\t\t ----------------------------------------------------------\n";
		cout << "\t\t\t\t Nema filmova u bazi podataka.";
	}
	else {
		prebrojiFilmove();
		film *filmovi = new film[brojFilmova];
		ucitajFilmove(filmovi);
		cout << "\t\t\t\t ----------------------------------------------------------\n";
        for(int i = 0; i < brojFilmova; i++) {
            cout << "\t\t\t\t | Ime filma: \t\t\t" << filmovi[i].naziv_filma <<"\n";
            cout << "\t\t\t\t | Zanr filma: \t\t\t" << filmovi[i].zanr <<"\n";
            cout << "\t\t\t\t | Format filma: \t\t" << filmovi[i].format <<"\n";
            cout << "\t\t\t\t | Datum izlaska filma: \t" << filmovi[i].datum_izlaska <<"\n";
            cout << "\t\t\t\t | Vrijeme trajanja filma: \t" << filmovi[i].trajanje_filma <<"\n";
            cout << "\t\t\t\t | Glavni glumac: \t\t" << filmovi[i].glavni_glumac <<"\n";
            cout << "\t\t\t\t | Reziser: \t\t\t" << filmovi[i].reziser <<"\n";
            cout << "\t\t\t\t | Jezik: \t\t\t" << filmovi[i].jezik <<"\n";
            cout << "\t\t\t\t ------------------------------------------------\n";
        }
		delete[] filmovi;
	}
}

void ucitajFilmove(film *Filmovi) {
	
	ifstream datoteka;
	datoteka.open("Baza Podataka/Filmovi.txt", ios::in);
	
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {		
	
		string temp;
	
		for(int i = 0; i < brojFilmova; i++) {	
			getline(datoteka, temp);
			strcpy(Filmovi[i].naziv_filma, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].zanr, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].format, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].datum_izlaska, temp.c_str());			
			getline(datoteka, temp);
			strcpy(Filmovi[i].glavni_glumac, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].reziser, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].mjesto_radnje, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].direktor, temp.c_str());
			getline(datoteka, temp);
			strcpy(Filmovi[i].jezik, temp.c_str());
			datoteka >> Filmovi[i].trajanje_filma;
			datoteka.ignore();
		}
		datoteka.close();
	} 
}

void upisiFilmove(film *Filmovi) {
	
	ofstream datoteka;
	datoteka.open("Baza Podataka/Filmovi.txt");
	
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {		
		for(int i = 0; i < brojFilmova; i++) {	
			datoteka << Filmovi[i].naziv_filma << endl;
			datoteka << Filmovi[i].zanr << endl;
			datoteka << Filmovi[i].format << endl;
			datoteka << Filmovi[i].datum_izlaska << endl;
			datoteka << Filmovi[i].glavni_glumac << endl;
			datoteka << Filmovi[i].reziser << endl;
			datoteka << Filmovi[i].mjesto_radnje << endl;
			datoteka << Filmovi[i].direktor << endl;
			datoteka << Filmovi[i].jezik << endl;
			datoteka << Filmovi[i].trajanje_filma << endl;
		}
		datoteka.close();
	}	
}

void prebrojiFilmove() {
	
	ifstream datoteka;
	datoteka.open("Baza Podataka/Filmovi.txt");
	
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		string linija;
		while(getline(datoteka, linija)) brojFilmova++;	
		datoteka.close();
	}
	brojFilmova /= 10;
}

void UbaciUSalu() {

	if(brojSala == 8) {
		cout << "\n\t\t\t\t Greska - Sve sale su popunjene." << endl;
		return;
	} else {
		cout << brojSala;
		film *filmovi = new film[brojFilmova];
		sala sale;
					
		ucitajFilmove(filmovi);
		system("cls");
		cout << "\t\t\t\t ----------------------------------------------------------\n";
		cout << "\t\t\t\t   _   _  _               _    __  _  _        \n";
		cout << "\t\t\t\t  | | | || |__  __ _  __ (_)  / _|(_)| | _ __  \n";
		cout << "\t\t\t\t  | |_| || '_ \\/ _` |/ _|| | |  _|| || || '  \\ \n";
		cout << "\t\t\t\t   \\___/ |_.__/\\__,_|\\__||_| |_|  |_||_||_|_|_|\n\n";
		prikazi_filmove();		
		cout << "\t\t\t\t  Unesite redni broj filma: ";
		int redniBroj, pokusaji = 0;
		do {
			cin >> redniBroj;
			if(redniBroj < 1 || redniBroj > brojFilmova) {
				cout << "\t\t\t\t  Greska - Redni broj filmova mora biti u rasponu od 1-" << brojFilmova << ".\n";
				if(pokusaji == 5) {
					cout << "\t\t\t\t  Previse pokusaja!\n";
					cout << "\t\t\t\t  Sistem se vraca u glavni program.";
					main();
				}
				cout << "\t\t\t\t  Ponovo unesite broj filma: ";
			}
			pokusaji++;
		} while(redniBroj < 1 || redniBroj > brojFilmova);
	
		strcpy(sale.film.naziv_filma, filmovi[redniBroj-1].naziv_filma);
		strcpy(sale.film.zanr, filmovi[redniBroj-1].zanr);
		strcpy(sale.film.format, filmovi[redniBroj-1].format);
		strcpy(sale.film.datum_izlaska, filmovi[redniBroj-1].datum_izlaska);
		strcpy(sale.film.glavni_glumac, filmovi[redniBroj-1].glavni_glumac);
		strcpy(sale.film.reziser, filmovi[redniBroj-1].reziser);
		strcpy(sale.film.mjesto_radnje, filmovi[redniBroj-1].mjesto_radnje);
		strcpy(sale.film.direktor, filmovi[redniBroj-1].direktor);
		strcpy(sale.film.jezik, filmovi[redniBroj-1].jezik);		
		sale.film.trajanje_filma = filmovi[redniBroj-1].trajanje_filma;
		cin.ignore();
		cout << "\t\t\t\t  Unesite vrijeme prezentovanja filma (h:mPM/AM): ";
		do {
			cin.getline(sale.show_time, 8);
			if(strlen(sale.show_time) == 0) {
				cout << "\t\t\t\t  Morate nesto unijeti." << endl;
				cout << "\t\t\t\t  Ponovo unesite vrijeme (h:mPM/AM): ";
			}
		} while(strlen(sale.show_time) == 0);
		cout << "\t\t\t\t  Unesite broj sale: ";
		int brojSale;
		do {
			cin >> brojSale;
			if(brojSale < 1 || brojSale > MAX_SALA) {
				cout << "\t\t\t\t  Greska - Broj Sale treba da bude od 1-8.";
				cout << "\n\t\t\t\t  Ponovo unesite broj sale: ";
			}
		} while(brojSale < 1 || brojSale > MAX_SALA);
		cout << "\t\t\t\t  Unesite cijenu karte: ";
		do {
			cin >> sale.cijena;
			if(sale.cijena < 0) {
				cout << "\t\t\t\t  Greska - Cijena karte ne moze biti negativna." << endl;
				cout << "\t\t\t\t  Ponovo unesite cijenu karte: ";
			}
			if(sale.cijena > 10) {
				cout << "\t\t\t\t  Greska - Cijena karte ne moze biti veca od 10KM" << endl;
				cout << "\t\t\t\t  Ponovo unesite cijenu karte: ";
			}
		} while(sale.cijena < 0 || sale.cijena > 10);
		sale.broj_sale = brojSale;
		sale.slobodna_sjedista = 100;
		UpisiSalu(sale, brojSale);
		delete[] filmovi;	
	}
}

void UpisiSalu(sala Sale, int brojSale) {

	ofstream datoteka;
	datoteka.open("Baza Podataka/Sale.txt", ios::app);

	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		datoteka << Sale.film.naziv_filma << endl;
		datoteka << Sale.film.zanr << endl;
		datoteka << Sale.film.format << endl;
		datoteka << Sale.film.datum_izlaska << endl;
		datoteka << Sale.film.glavni_glumac << endl;
		datoteka << Sale.film.reziser << endl;
		datoteka << Sale.film.mjesto_radnje << endl;
		datoteka << Sale.film.direktor << endl;
		datoteka << Sale.film.jezik << endl;
		datoteka << Sale.show_time << endl;
		datoteka << Sale.film.trajanje_filma << endl;
		datoteka << Sale.broj_sale << endl;
		datoteka << Sale.broj_sjedista << endl;
		datoteka << Sale.slobodna_sjedista << endl;
		datoteka << Sale.cijena << endl;
		datoteka.close();
	}
	
	string imedatoteke_sjedista = "Sjedista_sala_" + to_string(brojSale) + ".txt";
    datoteka.open("Baza Podataka/" + imedatoteke_sjedista);
    if(datoteka.fail()) cout << "Greska - Nije moguce otvoriti datoteku.";
    else {
        for(int i = 1; i <= 100; i+=10) {
            for(int j = 0; j < 10; j++) {
                datoteka<< i+j << " ";
            }
            datoteka << endl;
        }
        datoteka.close();
    }
    prebrojiSale();
}

void UpisiSale(sala *Sale) {
	
	ofstream datoteka;
	datoteka.open("Baza Podataka/Sale.txt");
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		for(int i = 0; i < brojSala; i++) {
			datoteka << Sale[i].film.naziv_filma << endl;
			datoteka << Sale[i].film.zanr << endl;
			datoteka << Sale[i].film.format << endl;
			datoteka << Sale[i].film.datum_izlaska << endl;
			datoteka << Sale[i].film.glavni_glumac << endl;
			datoteka << Sale[i].film.reziser << endl;
			datoteka << Sale[i].film.mjesto_radnje << endl;
			datoteka << Sale[i].film.direktor << endl;
			datoteka << Sale[i].film.jezik << endl;
			datoteka << Sale[i].show_time << endl;
			datoteka << Sale[i].film.trajanje_filma << endl;
			datoteka << Sale[i].broj_sale << endl;
			datoteka << Sale[i].broj_sjedista << endl;
			datoteka << Sale[i].slobodna_sjedista << endl;
			datoteka << Sale[i].cijena << endl;				
		}
		datoteka.close();
	}	
}

void ResetujSale() {
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t   ___                _               _  _ \n";
	cout << "\t\t\t\t  | _ \\ ___  ___ ___ | |_   ___ __ _ | |(_)\n";
	cout << "\t\t\t\t  |   // -_)(_-</ -_)|  _| (_-</ _` || || |\n";
	cout << "\t\t\t\t  |_|_\\\\___|/__/\\___| \\__| /__/\\__,_||_||_|\n";
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	ofstream datoteka;
	datoteka.open("Baza Podataka/Sale.txt", ofstream::out | ofstream::trunc);
	datoteka.close();
	for(int i = 0; i < MAX_SALA; i++) {
		string imedatoteke_sjedista = "Sjedista_sala_" + to_string(i+1) + ".txt";
		datoteka.open("Baza Podataka/" + imedatoteke_sjedista, ofstream::out | ofstream::trunc);
		for(int k = 1; k <= 100; k+=10) {
	        for(int j = 0; j < 10; j++) {
	            datoteka<< k+j << " ";
	        }
	        datoteka << endl;
	    }
	    datoteka.close();
	}
	prebrojiSale();
}

void prebrojiSale() {
	
	ifstream datoteka;
	datoteka.open("Baza Podataka/Sale.txt");
	
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		string linija;
		brojSala = 0;
		while(getline(datoteka, linija)) brojSala++;	
		datoteka.close();
	}
	brojSala /= 15;
}

void IzmjeniSalu(int brojSale, int **matrica) {	
	
	string imedatoteke_sjedista = "Sjedista_sala_" + to_string(brojSale) + ".txt";
	
	ofstream datoteka;
	datoteka.open("Baza Podataka/" + imedatoteke_sjedista);
	if(datoteka.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 10; j++) {
				datoteka << matrica[i][j] << " ";
			}
			datoteka << endl;
		}
		datoteka.close();
	}
}

void UcitajSjedista(int brojSale, int **matrica) {
	
	string imedatoteke_sjedista = "Sjedista_sala_" + to_string(brojSale) + ".txt";
	
	ifstream datoteka_sjedista;
	datoteka_sjedista.open("Baza Podataka/" + imedatoteke_sjedista);
	if(datoteka_sjedista.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {
		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 10; j++) {
				datoteka_sjedista >> matrica[i][j];
			}
		}
		datoteka_sjedista.close();
	}
}

void UcitajSale(sala *Sale) {

	ifstream datoteka_sala;
	datoteka_sala.open("Baza Podataka/Sale.txt");
	if(datoteka_sala.fail()) cout << "\t\t\t\t Greska - Nije moguce otvoriti datoteku.";
	else {	
		string temp;
		for(int i = 0; i < brojSala; i++) {
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.naziv_filma, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.zanr, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.format, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.datum_izlaska, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.glavni_glumac, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.reziser, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.mjesto_radnje, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.direktor, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].film.jezik, temp.c_str());
			getline(datoteka_sala, temp);
			strcpy(Sale[i].show_time, temp.c_str());
			datoteka_sala >> Sale[i].film.trajanje_filma;				
			datoteka_sala >> Sale[i].broj_sale;
			datoteka_sala >> Sale[i].broj_sjedista;
			datoteka_sala >> Sale[i].slobodna_sjedista;	
			datoteka_sala >> Sale[i].cijena;
			datoteka_sala.ignore();
		}
		datoteka_sala.close();	
		prebrojiSale();
	}
}

void PrikaziSalu(sala *sale) {
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t   ___            _        __  _  _              \n";
	cout << "\t\t\t\t  |_ _| ___ _ __ (_) ___  / _|(_)| | _ __   __ _ \n";
	cout << "\t\t\t\t   | | (_-<| '_ \\| |(_-< |  _|| || || '  \\ / _` |\n";
	cout << "\t\t\t\t  |___|/__/| .__/|_|/__/ |_|  |_||_||_|_|_|\\__,_|\n";
	cout << "\t\t\t\t           |_|                                   \n";
    cout << "\t\t\t\t ----------------------------------------------------------\n";
    cout << "\t\t\t\t  Ime filma: "                    << sale -> film.naziv_filma << endl;
    cout << "\t\t\t\t  Zanr filma: "                   << sale -> film.zanr << endl;
    cout << "\t\t\t\t  Format filma: "                 << sale -> film.format << endl;
    cout << "\t\t\t\t  Datum izlaska: "                << sale -> film.datum_izlaska << endl;
    cout << "\t\t\t\t  Glavni glumac: "                << sale -> film.glavni_glumac << endl;
    cout << "\t\t\t\t  Reziser: "                      << sale -> film.reziser << endl;
    cout << "\t\t\t\t  Mjesto radnje filma: "          << sale -> film.mjesto_radnje << endl;
    cout << "\t\t\t\t  Direktor filma: "               << sale -> film.direktor << endl;
    cout << "\t\t\t\t  Jezik filma: "                  << sale -> film.jezik << endl;
    cout << "\t\t\t\t  Trajanje filma: "               << sale -> film.trajanje_filma << endl;
    cout << "\t\t\t\t  Vrijeme prezentovanja filma: "  << sale -> show_time << endl;
    cout << "\t\t\t\t  Broj sjedista u sali: "         << sale -> broj_sjedista << endl;
    cout << "\t\t\t\t  Broj sale: "                    << sale -> broj_sale << endl;
    cout << "\t\t\t\t  Broj slobodnih sjedista: "      << sale -> slobodna_sjedista << endl;
    cout << "\t\t\t\t  Cijena karte: "                 << sale -> cijena << endl;
    cout << "\t\t\t\t ----------------------------------------------------------\n";
}

void loading(const char *poruka) {
	
	char *recenica = new char[strlen(poruka) + 1];
	strcpy(recenica, poruka);
	cout << "\n\t\t\t\t " << recenica << "\n";
	unsigned char znak = 219;
	cout << "\t\t\t\t  ";
	for(int i = 0; i < 42; i++) {
		cout << znak;
		Sleep(80);
	}
}

int korisnicki_izbor() {
	system("cls");
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t           ___    ____     ____      _      ____  \n";
	cout << "\t\t\t\t          / _ \\  / ___|   / ___|    / \\    |  _ \\ \n";
	cout << "\t\t\t\t         | | | | \\___ \\  | |       / _ \\   | |_) |\n";
	cout << "\t\t\t\t         | |_| |  ___) | | |___   / ___ \\  |  _ < \n";
	cout << "\t\t\t\t          \\___/  |____/   \\____| /_/   \\_\\ |_| \\_\\\n";
	cout << "\t\t\t\t\n";
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t  1. Kupi kartu." << endl;
	cout << "\t\t\t\t  2. Prikazi film." << endl;
	cout << "\t\t\t\t  3. Administratorski meni." << endl;
	cout << "\t\t\t\t  4. Izadji iz sistema." << endl;
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout <<"\t\t\t\t  Unesite opciju: ";
	
	int izbor;
	do {
		cin >> izbor;
		if(izbor < 1) {
			cout << "\t\t\t\t  Greska - Ne mozete unijeti opciju manju od 1.\n";
			cout << "\t\t\t\t  Ponovo unesite opciju: ";
		} else if (izbor > 4) {
			cout << "\t\t\t\t  Greska - Ne mozete unijeti opciju vecu od 5.\n";
			cout << "\t\t\t\t  Ponovo unesite opciju: ";			
		}
	} while(izbor < 1 || izbor > 4);
	return izbor;
}

int administratorski_izbor() {
	system("cls");
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t     _       _         _                           _ \n";
	cout << "\t\t\t\t    /_\\   __| | _ __  (_) _ _    _ __   ___  _ _  (_)\n";
	cout << "\t\t\t\t   / _ \\ / _` || '  \\ | || ' \\  | '  \\ / -_)| ' \\ | |\n";
	cout << "\t\t\t\t  /_/ \\_\\\\__,_||_|_|_||_||_||_| |_|_|_|\\___||_||_||_|\n\n";
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t  1. Dodaj novi film." << endl;
	cout << "\t\t\t\t  2. Uredi film." << endl;
	cout << "\t\t\t\t  3. Izbrisi film." << endl;
	cout << "\t\t\t\t  4. Prikazi filmove." << endl;
	cout << "\t\t\t\t  5. Ubaci film u salu." << endl;
	cout << "\t\t\t\t  6. Resetuj sale." << endl;
	cout << "\t\t\t\t  7. Izadji iz Administratorskog menija." << endl;
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	int izbor;
	do {
		cout<<"\t\t\t\t  Unesite opciju: ";
		cin >> izbor;
		if(izbor < 1) {
			cout << "\t\t\t\t  Greska - Ne mozete unijeti opciju manju od 1.";
			cout << "\n\t\t\t\t  Ponovo unesite opciju: ";
		} else if (izbor > 7) {
			cout << "\t\t\t\t  Greska - Ne mozete unijeti opciju manju od 7.";
			cout << "\n\t\t\t\t  Ponovo unesite opciju: ";			
		}
	} while(izbor < 1 || izbor > 7);		
	return izbor;
}

int uredjivanjeFilmaMeni() {
	
	system("cls");
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t   _   _               _   _  _                    _      \n";
	cout << "\t\t\t\t  | | | | _ _  ___  __| | (_)(_)__ __ __ _  _ _   (_) ___ \n";
	cout << "\t\t\t\t  | |_| || '_|/ -_)/ _` | | || |\\ V // _` || ' \\  | |/ -_)\n";
	cout << "\t\t\t\t   \\___/ |_|  \\___|\\__,_|_/ ||_| \\_/ \\__,_||_||_|_/ |\\___|\n";
	cout << "\t\t\t\t                        |__/                    |__/      \n";	
	cout << "\t\t\t\t                                    __  _  _              \n";	
	cout << "\t\t\t\t                                   / _|(_)| | _ __   __ _ \n";	
	cout << "\t\t\t\t                                  |  _|| || || '  \\ / _` |\n";	
	cout << "\t\t\t\t                                  |_|  |_||_||_|_|_|\\__,_|\n";	
	cout << "\t\t\t\t ----------------------------------------------------------\n";
	cout << "\t\t\t\t  Uredjivanje filma:" << endl;
	cout << "\t\t\t\t  1. Novi naziv filma." << endl;
	cout << "\t\t\t\t  2. Novi zanr filma." << endl;
	cout << "\t\t\t\t  3. Novi format filma." << endl;
	cout << "\t\t\t\t  4. Novi datum izlaska filma." << endl;
	cout << "\t\t\t\t  5. Novo trajanje filma." << endl;
	cout << "\t\t\t\t  6. Novo ime glavnog glumca." << endl;
	cout << "\t\t\t\t  7. Novi reziser filma." << endl; 
	cout << "\t\t\t\t  8. Novo mjesto radnje filma." << endl;
	cout << "\t\t\t\t  9. Novi direktor filma." << endl;
	cout << "\t\t\t\t  10. Novi jezik filma." << endl;     
	cout << "\t\t\t\t  11. Spremi izmjene." << endl;
	cout << "\t\t\t\t  12. Izadji iz uredjivanja filma." << endl;
	cout << "\t\t\t\t  Unesite opciju: ";
	int izbor;
	do {
		cin >> izbor;
		if(izbor < 1 || izbor > 12) {
			cout << "\t\t\t\t  Greska - Opcija mora biti od 1 do 12." << endl;
			cout << "\t\t\t\t  Ponovo unesi opciju: ";
		}
	}while(izbor < 1 || izbor > 12);
	return izbor;
}

int suma(int cijena, int brojKarti) {
	
	if(brojKarti == 1) return cijena;
	else return cijena + suma(cijena, brojKarti-1);
}

bool provjera(string username, string password) {
	
	if(username == "zijad" && password == "123456") {
		return true;
	} else if(username == "zijad" && password != "123456") {
		cout << "\t\t\t\t  Netacna lozinka" << endl;
		return false;
	} else if(username == "muhamed" && password == "55555") {
		return true;
	} else if(username == "muhamed" && password != "55555") {
		cout << "\t\t\t\t  Netacna lozinka" << endl;
		return false;		
	} else if(username == "samed" && password == "98765") {
		return true;
	} else if(username == "samed" && password != "98765") {
		cout << "\t\t\t\t  Netacna lozinka" << endl;
		return false;
	} else {
		cout << "\t\t\t\t  Ne postoji admin sa tim podacima u sistemu." << endl;
		return false;
	}
}

sala *ucitaj_salu(int brojFilma) {
	
	sala *sale = new sala[brojSala];
	UcitajSale(sale);
	return &sale[brojFilma];
}

