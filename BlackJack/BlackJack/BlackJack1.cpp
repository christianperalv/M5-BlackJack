#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Estructura para representar una carta
struct Carta {
    string palo;
    string valor;
    int puntos;
};

// Función para crear una baraja de cartas
vector<Carta> crearBaraja() {
    vector<Carta> baraja;
    string palos[] = { "Corazones", "Diamantes", "Tréboles", "Picas" };
    string valores[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    // Generar la baraja
    for (string palo : palos) {
        for (string valor : valores) {
            int puntos = (valor == "A") ? 11 : (valor == "J" || valor == "Q" || valor == "K") ? 10 : stoi(valor);
            baraja.push_back({ palo, valor, puntos });
        }
    }

    // Barajar la baraja
    srand(time(0));
    for (int i = 0; i < baraja.size(); ++i) {
        int j = rand() % baraja.size();
        swap(baraja[i], baraja[j]);
    }

    return baraja;
}

// Función para calcular el valor de la mano
int calcularValorMano(const vector<Carta>& mano) {
    int suma = 0;
    int num_ases = 0;
    for (const Carta& carta : mano) {
        suma += carta.puntos;
        if (carta.valor == "A") {
            num_ases++;
        }
    }
    while (suma > 21 && num_ases > 0) {
        suma -= 10;
        num_ases--;
    }
    return suma;
}

// Función para mostrar la mano del jugador o del croupier
void mostrarMano(const vector<Carta>& mano, bool ocultarPrimeraCarta = false) {
    for (int i = 0; i < mano.size(); ++i) {
        if (i == 0 && ocultarPrimeraCarta) {
            cout << "[Oculta]";
        }
        else {
            cout << mano[i].valor << " de " << mano[i].palo;
        }
        if (i != mano.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

// Función para realizar el turno del jugador
void turnoJugador(vector<Carta>& manoJugador, vector<Carta>& baraja) {
    char eleccion;
    while (true) {
        cout << "Tu mano: ";
        mostrarMano(manoJugador);
        int valorMano = calcularValorMano(manoJugador);
        cout << "Valor actual de la mano: " << valorMano << endl;
        if (valorMano >= 21) {
            break;
        }
        cout << "¿Quieres pedir carta (h) o plantarte (s)? ";
        cin >> eleccion;
        if (eleccion == 'h') {
            manoJugador.push_back(baraja.back());
            baraja.pop_back();
        }
        else if (eleccion == 's') {
            break;
        }
    }
}

// Función para realizar el turno del croupier
void turnoCroupier(vector<Carta>& manoCroupier, vector<Carta>& baraja) {
    while (calcularValorMano(manoCroupier) < 17) {
        manoCroupier.push_back(baraja.back());
        baraja.pop_back();
    }
}

// Función principal del juego
void jugarBlackjack() {
    vector<Carta> baraja = crearBaraja();

    vector<Carta> manoJugador;
    vector<Carta> manoCroupier;

    // Repartir cartas iniciales
    manoJugador.push_back(baraja.back());
    baraja.pop_back();
    manoCroupier.push_back(baraja.back());
    baraja.pop_back();
    manoJugador.push_back(baraja.back());
    baraja.pop_back();

    cout << "Tu mano: ";
    mostrarMano(manoJugador);
    cout << "Mano del croupier: ";
    mostrarMano(manoCroupier, true);

    // Turno del jugador
    turnoJugador(manoJugador, baraja);

    // Verificar si el jugador se ha pasado de 21
    if (calcularValorMano(manoJugador) > 21) {
        cout << "¡Te has pasado! El croupier gana." << endl;
        return;
    }

    // Turno del croupier
    turnoCroupier(manoCroupier, baraja);

    // Mostrar las manos finales
    cout << "Tu mano: ";
    mostrarMano(manoJugador);
    cout << "Mano del croupier: ";
    mostrarMano(manoCroupier);

    // Determinar el resultado
    int valorJugador = calcularValorMano(manoJugador);
    int valorCroupier = calcularValorMano(manoCroupier);
    if (valorJugador == valorCroupier) {
        cout << "¡Es un empate!" << endl;
    }
    else if (valorJugador == 21 || (valorCroupier > 21 || valorJugador > valorCroupier)) {
        cout << "¡Felicidades! ¡Has ganado!" << endl;
    }
    else {
        cout << "¡El croupier gana!" << endl;
    }
}

int main() {
    char jugarOtraVez;
    do {
        jugarBlackjack();
        cout << "¿Quieres jugar otra vez? (s/n): ";
        cin >> jugarOtraVez;
    } while (jugarOtraVez == 's' || jugarOtraVez == 'S');
    return 0;
}
