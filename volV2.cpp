#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // Pour la fonction system("cls")
#include <algorithm> // Pour la fonction std::transform
using namespace std;

void clearScreen() {
    system("cls");
}

class Destination {
public:
    string nom;
    string description;
    double coutMoyen;

    Destination(string n, string desc, double cm) : nom(n), description(desc), coutMoyen(cm) {}

    void afficherDetails() {
        cout << "Nom de la destination : " << nom << endl;
        cout << "Description : " << description << endl;
        cout << "Cout moyen du voyage : " << coutMoyen << " DH" << endl;
    }
};

class Transport {
protected:
    string mode;
    string compagnie;
    double coutTransport;

public:
    Transport(string m, string comp, double c) :
        mode(m), compagnie(comp), coutTransport(c) {}

    virtual void afficher() {
        cout << "Mode de transport : " << mode << endl;
        cout << "Compagnie : " << compagnie << endl;
        cout << "Cout : " << coutTransport << " DH" << endl;
    }

    string getMode() const {
        return mode;
    }
};

class Avion : public Transport {
public:
    Avion(string compagnie, double cout) : Transport("Avion", compagnie, cout) {}
};

class Train : public Transport {
public:
    Train(string compagnie, double cout) : Transport("Train", compagnie, cout) {}
};

class Voyage {
protected:
    int idVoyage;
    Destination destination;
    string dateDepart;
    double prix;
    int nombrePlaces;
    Transport* transport;

public:
    Voyage(int id, Destination dest, string date, double pri, int places, Transport* trans)
        : idVoyage(id), destination(dest), dateDepart(date), prix(pri), nombrePlaces(places), transport(trans) {}

    virtual ~Voyage() {
        delete transport;
    }

    int getIdVoyage() const {
        return idVoyage;
    }

    const Destination& getDestination() const {
        return destination;
    }

    const string& getDateDepart() const {
        return dateDepart;
    }

    virtual void afficher() {
        cout << "ID du voyage : " << idVoyage << endl;
        cout << "Destination : " << destination.nom << endl;
        cout << "Date de depart : " << dateDepart << endl;
        cout << "Prix : " << prix << " DH" << endl;
        cout << "Nombre de places disponibles : " << nombrePlaces << endl;
        transport->afficher();
    }

    virtual void reserver(int nombrePersonnes, Transport* choixTransport) {
        if (nombrePersonnes <= nombrePlaces) {
            nombrePlaces -= nombrePersonnes;
            cout << nombrePersonnes << (nombrePersonnes > 1 ? " places" : " place") << " reservee" << (nombrePersonnes > 1 ? "s" : "") << " pour le voyage " << idVoyage << endl;
            cout << "Mode de transport choisi : " << endl;
            choixTransport->afficher();

            cout << "Voulez-vous ajouter une excursion supplementaire ? (Oui/Non): ";
            string choix;
            cin >> choix;
            if (choix == "Oui" || choix == "oui") {
                ajouterExcursion();
            }
        } else {
            cout << "Desole, plus de places disponibles pour le voyage " << idVoyage << endl;
        }

        if (nombrePlaces < 1) {
            cout << "Attention, plus aucune place disponible pour le voyage " << idVoyage << endl;
        }
    }

    void ajouterExcursion() {
        cout << "Excursions disponibles pour ce voyage :" << endl;
        cout << "Voulez-vous ajouter cette excursion ? (Oui/Non): ";
        string confirmation;
        cin >> confirmation;
        if (confirmation == "Oui" || confirmation == "oui") {
            cout << "Excursion ajoutee avec succes !" << endl;
        } else {
            cout << "Excursion non ajoutee." << endl;
        }
    }

    virtual void annuler(int nombrePersonnes) {
        nombrePlaces += nombrePersonnes;
        cout << nombrePersonnes << " place(s) annulee(s) pour le voyage " << idVoyage << endl;
    }
    
};

class Client {
private:
    string nom;
    string adresseEmail;
    vector<Voyage*> voyagesReserves;

public:
    Client(string n, string email) : nom(n), adresseEmail(email) {}

    void reserverVoyage(Voyage* voyage, int nombrePersonnes, Transport* choixTransport) {
        voyage->reserver(nombrePersonnes, choixTransport);
        voyagesReserves.push_back(voyage);
    }

    void annulerReservation(Voyage* voyage, int nombrePersonnes) {
        voyage->annuler(nombrePersonnes);
        for (auto it = voyagesReserves.begin(); it != voyagesReserves.end(); ++it) {
            if ((*it)->getIdVoyage() == voyage->getIdVoyage()) {
                voyagesReserves.erase(it);
                break;
            }
        }
    }

    void afficherDetails() {
        cout << "Nom du client : " << nom << endl;
        cout << "Adresse email : " << adresseEmail << endl;
        cout << "Voyages reserves :" << endl;
        for (auto& voyage : voyagesReserves) {
            voyage->afficher();
            cout << endl;
        }
    }
};

class AgenceVoyage {
public :
    vector<Voyage*> voyagesDisponibles;
    vector<Client*> clients;

public:
    ~AgenceVoyage() {
        for (auto& voyage : voyagesDisponibles) {
            delete voyage;
        }
        for (auto& client : clients) {
            delete client;
        }
    }

    void ajouterVoyage(Voyage* voyage) {
        voyagesDisponibles.push_back(voyage);
    }

    void ajouterClient(Client* client) {
        clients.push_back(client);
    }

    void rechercherVoyage(string destination) {
        bool voyageTrouve = false;
        cout << "Voyages disponibles pour la destination " << destination << endl;
        for (auto& voyage : voyagesDisponibles) {
            if (voyage->getDestination().nom == destination) {
                voyage->afficher();
                cout << "----------------------------------------------" << endl;
                voyageTrouve = true;
            }
        }
        if (!voyageTrouve) {
            cout << "Aucun voyage trouvé pour la destination " << destination << endl;
        }
    }

    bool traiterReservation(int idVoyage, int nombrePersonnes, Transport* choixTransport) {
        bool voyageExiste = false;
        for (auto& voyage : voyagesDisponibles) {
            if (voyage->getIdVoyage() == idVoyage) {
                voyage->reserver(nombrePersonnes, choixTransport);
                voyageExiste = true;
                break;
            }
        }
        if (!voyageExiste) {
            cout << "Le voyage avec l'ID " << idVoyage << " n'existe pas." << endl;
        }
        return voyageExiste;
    }

    void annulerReservation(int idVoyage, int nombrePersonnes) {
        bool voyageExiste = false;
        for (auto& voyage : voyagesDisponibles) {
            if (voyage->getIdVoyage() == idVoyage) {
                voyage->annuler(nombrePersonnes);
                voyageExiste = true;
                break;
            }
        }
        if (!voyageExiste) {
            cout << "Le voyage avec l'ID " << idVoyage << " n'existe pas." << endl;
        }
    }

    void afficherVoyages() {
        cout << "Voyages disponibles :" << endl;
        for (auto& voyage : voyagesDisponibles) {
            voyage->afficher();
            cout << "----------------------------------------------" << endl;
        }
    }
};

class Forfait : public Voyage {
private:
    string nomForfait;
    string description;
    vector<Voyage*> voyagesInclus;

public:
    Forfait(int idVoyage, Destination destination, string dateDepart, double prix, int nombrePlaces, string nomForfait, string description)
        : Voyage(idVoyage, destination, dateDepart, prix, nombrePlaces, new Avion("Air France", 300.0)), nomForfait(nomForfait), description(description) {}

    void ajouterVoyageInclus(Voyage* voyage) {
        voyagesInclus.push_back(voyage);
    }

    void afficher() override {
        Voyage::afficher();
        cout << "Nom du forfait : " << nomForfait << endl;
        cout << "Description : " << description << endl;
        cout << "Voyages inclus dans le forfait :" << endl;
        for (auto& voyage : voyagesInclus) {
            voyage->afficher();
            cout << "----------------------------------------------" << endl;
        }
    }
};

class GuideTouristique : public AgenceVoyage {
private:
    string nom;
    string languesParlees;
    int experience;

public:
    GuideTouristique(string n, string lp, int exp)
        : nom(n), languesParlees(lp), experience(exp) {}

    void afficher() {
        cout << "Nom du guide : " << nom << endl;
        cout << "Langues parlees : " << languesParlees << endl;
        cout << "Experience : " << experience << " ans" << endl;
    }
};

class Excursion : public Voyage {
public:
    Excursion(int idVoyage, Destination destination, string dateDepart, double prix, int nombrePlaces, Transport* transport)
        : Voyage(idVoyage, destination, dateDepart, prix, nombrePlaces, transport) {}

    void afficher() override {
        Voyage::afficher();
        cout << "Type : Excursion" << endl;
    }
};

int main() {
    AgenceVoyage agence;

    Destination paris("Paris", "La ville de l'amour et des lumieres", 2000.0);
    Destination londres("Londres", "La capitale du Royaume-Uni", 2500.0);
    Destination newyork("New York", "La ville qui ne dort jamais", 3000.0);

    Voyage* voyage1 = new Voyage(1, paris, "01/05/2024", 1500.0, 50, new Avion("Air Maroc", 300.0));
    Voyage* voyage2 = new Voyage(2, londres, "15/06/2024", 1800.0, 40, new Train("ONCF", 200.0));
    Voyage* voyage3 = new Voyage(3, newyork, "20/07/2024", 2500.0, 60, new Avion("Air Arabia", 400.0));

    Forfait* forfait1 = new Forfait(4, paris, "10/08/2024", 3000.0, 20, "Romance a Paris", "Decouvrez la ville de l'amour avec ce forfait romantique");
    forfait1->ajouterVoyageInclus(voyage1);
    forfait1->ajouterVoyageInclus(voyage2);

    Excursion* excursion1 = new Excursion(5, londres, "25/09/2024", 1200.0, 15, new Train("ONCF", 200.0));
    Excursion* excursion2 = new Excursion(6, newyork, "30/09/2024", 1500.0, 20, new Avion("Air Maroc", 300.0));

    agence.ajouterVoyage(voyage1);
    agence.ajouterVoyage(voyage2);
    agence.ajouterVoyage(voyage3);
    agence.ajouterVoyage(forfait1);
    agence.ajouterVoyage(excursion1);
    agence.ajouterVoyage(excursion2);

    // Ajout des clients
    Client client1("Jean Dupont", "jean.dupont@example.com");
    Client client2("Alice Martin", "alice.martin@example.com");

    agence.ajouterClient(&client1);
    agence.ajouterClient(&client2);

    int choix;
    do {
        clearScreen();
        cout << "Menu :" << endl;
        cout << "1. Afficher les voyages disponibles" << endl;
        cout << "2. Reserver un voyage" << endl;
        cout << "3. Annuler une reservation" << endl;
        cout << "4. Rechercher un voyage par destination" << endl;
        cout << "5. Afficher les details d'un voyage par son ID" << endl;
        cout << "6. Ajouter un forfait au voyage" << endl;
        cout << "7. Afficher les details des clients" << endl;
        cout << "8. Quitter" << endl;
        cout << "Entrez votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1:
                agence.afficherVoyages();
                break;
           case 2:
    {
        clearScreen();
        int idVoyage, nombrePersonnes;
        cout << "Entrez l'ID du voyage : ";
        cin >> idVoyage;

        // Rechercher le voyage par son ID
        Voyage* voyage = nullptr;
        for (auto& v : agence.voyagesDisponibles) {
            if (v->getIdVoyage() == idVoyage) {
                voyage = v;
                break;
            }
        }

        if (voyage == nullptr) {
            cout << "Aucun voyage trouvé avec l'ID " << idVoyage << endl;
            break;
        }

        cout << "Entrez le nombre de personnes : ";
        cin >> nombrePersonnes;

        cout << "Modes de transport disponibles :" << endl;
        cout << "1. Avion" << endl;
        cout << "2. Train" << endl;
        cout << "Entrez votre choix de transport : ";
        int choixTransport;
        cin >> choixTransport;
        Transport* transport;
        if (choixTransport == 1) {
            transport = new Avion("Air Maroc", 300.0);
        } else if (choixTransport == 2) {
            transport = new Train("ONCF", 200.0);
        } else {
            cout << "Choix invalide." << endl;
            break;
        }

        // Demander confirmation à l'utilisateur
        cout << "Confirmez-vous la réservation pour " << nombrePersonnes << " personnes ? (Oui/Non): ";
        string confirmation;
        cin >> confirmation;
        if (confirmation == "Oui" || confirmation == "oui") {
            // Ajouter le client au voyage
            Client* client;
            string nomClient, emailClient;
            cout << "Entrez votre nom : ";
            cin >> nomClient;
            cout << "Entrez votre adresse email : ";
            cin >> emailClient;
            client = new Client(nomClient, emailClient);
            agence.ajouterClient(client);
            client->reserverVoyage(voyage, nombrePersonnes, transport);
        } else {
            cout << "Reservation annulée." << endl;
        }
        break;
    }

            case 3:
                {
                    int idVoyage, nombrePersonnes;
                    cout << "Entrez l'ID du voyage a annuler : ";
                    cin >> idVoyage;
                    cout << "Entrez le nombre de places a annuler : ";
                    cin >> nombrePersonnes;
                    agence.annulerReservation(idVoyage, nombrePersonnes);
                    break;
                }
            case 4:
                {
                    string destination;
                    cout << "Entrez la destination a rechercher : ";
                    cin >> destination;
                    agence.rechercherVoyage(destination);
                    break;
                }
            case 5:
                {
                    int idVoyage;
                    cout << "Entrez l'ID du voyage pour afficher les details : ";
                    cin >> idVoyage;
                    bool found = false;
                    for (auto& voyage : agence.voyagesDisponibles) {
                        if (voyage->getIdVoyage() == idVoyage) {
                            voyage->afficher();
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Aucun voyage trouve avec l'ID " << idVoyage << endl;
                    }
                    break;
                }
            case 6:
                {
                    clearScreen();
                    int idVoyageForfait;
                    string nomForfait, descriptionForfait;
                    cout << "Entrez l'ID du voyage principal pour le forfait : ";
                    cin >> idVoyageForfait;
                    cout << "Entrez le nom du forfait : ";
                    cin.ignore(); // Clear input buffer
                    getline(cin, nomForfait);
                    cout << "Entrez la description du forfait : ";
                    getline(cin, descriptionForfait);

                    Forfait* forfait = new Forfait(idVoyageForfait, paris, "01/01/2025", 5000.0, 30, nomForfait, descriptionForfait);
                    forfait->ajouterVoyageInclus(voyage1);
                    forfait->ajouterVoyageInclus(voyage2);
                    forfait->ajouterVoyageInclus(voyage3);

                    agence.ajouterVoyage(forfait);
                    cout << "Forfait ajouté avec succès !" << endl;
                    break;
                }
            case 7:
                {
                    clearScreen();
                    cout << "Details des clients :" << endl;
                    for (auto& client : agence.clients) {
                        client->afficherDetails();
                        cout << "----------------------------------------------" << endl;
                    }
                    break;
                }
            case 8:
                cout << "Au revoir !" << endl;
                break;
            default:
                cout << "Choix invalide, veuillez ressayer." << endl;
        }
        system("pause"); // Attend une action de l'utilisateur pour continuer
    } while (choix != 8);

    // Libération de la mémoire
    delete voyage1;
    delete voyage2;
    delete voyage3;
    delete forfait1;
    delete excursion1;
    delete excursion2;

    return 0;
}

