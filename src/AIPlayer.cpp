# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta
const int ENTRADA_CASA = 8;
const int MAX_DIST = 74;

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const{
    if(profundidad == profundidad_max or actual.gameOver()){
        return heuristic(actual,jugador);
    }

    ParchisBros hijos = actual.getChildren();

    if(jugador == actual.getCurrentPlayerId()){
         
        for(auto it = hijos.begin(); it != hijos.end() and alpha < beta; ++it){

            double aux = Poda_AlfaBeta(*it, jugador, profundidad +1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
            if(aux > alpha){
                alpha = aux; 
                if(profundidad == 0){
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();
                }

                if(beta <= alpha){

                    return beta;
                    break;
                }
            }

        }

        return alpha;

    }else {

        for(auto it = hijos.begin(); it != hijos.end() and alpha < beta; ++it){

            double aux = Poda_AlfaBeta(*it, jugador, profundidad+1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
            if(aux < beta){
                beta = aux;

                if(beta <= alpha){
                    return alpha;
                }
            }
        }

        return beta;
    }

}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    //como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }

    
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    /*
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
    */

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    
}

double ValoracionDadoEspecial(int powerbar, const Parchis &estado, int jugador){
    double valor; 

    if(0 <= powerbar < 50){
        //Movimiento rápido
        valor = 7 + powerbar/7;
        //Es conveniente agregar un bonus, ya que evade barreras
    }else if((50 <= powerbar < 60 )or (70 <= powerbar < 75)){
        //Concha Roja
        //Depende de la posición de otras piezas
        
        valor = 40; 

    }else if(60 <= powerbar < 65){
        //Boom
        valor = -MAX_DIST;

    }else if(65 <= powerbar < 70){
        //Movimiento ultra rápido
        valor = 25;

    }else if(75 <= powerbar < 80){
        //movimiento bala
        valor = 40;

    }else if(80 <= powerbar < 85){
        //Catapum
        valor = -2*MAX_DIST;

    }else if(85 <= powerbar < 90){
        //Concha Azul 
        //Arreglar
        vector<color> v = estado.getPlayerColors((jugador + 1)%2); 
        int min = 200; 
        int aux; 
        int j;
        color min_color;  

        for(int i = 0; i < 3; i++){
            aux = estado.distanceToGoal(v.at(0),i);
            if(aux < min and aux != 0) {
                min = aux;
                min_color = v.at(0);
                j = i;

            }
            aux = estado.distanceToGoal(v.at(1),i);
            if(aux < min and aux != 0){
                min = aux;
                min_color = v.at(1); 
                j = i;
            }
        }

        if(estado.isWall(estado.getBoard().getPiece(min_color,j).get_box())){
            valor = 2*MAX_DIST - 2*aux;
        }else{
            valor = MAX_DIST - aux; 
        }   
        

    }else if(90 <= powerbar < 95){
        //BoomBoom
        valor = -3*MAX_DIST;

    }else if(95 <= powerbar < 100){
        //Movimiento estrella
        //Difícil de evaluar
        valor = 150; 

    }else if(powerbar == 100){
        //Catapumchimpum
        valor = -4*MAX_DIST;

    }

    return valor;
}


double Distancia(const Parchis &estado, int jugador, vector<color> &mis_colores){
    //Posición relativa a la meta y la cercanía de las fichas
    //No del todo facil de evaluar
    //Sensible a picos
    //Influye el número de piezas en el tablero (posiciones de Zugzwang)

    double suma_1 = 0;
    double suma_2 = 0;

    //suma de distancias

    for(int i = 0; i < 3; i++){
        suma_1 += estado.distanceToGoal(mis_colores.at(0),i);
        suma_2 += estado.distanceToGoal(mis_colores.at(1),i);
    }

    //Desviación
    double var_1 = 0; 
    double var_2 = 0;
    for(int i = 0; i < 3; i++){
        var_1 += pow(suma_1/3.0 - estado.distanceToGoal(mis_colores.at(0),i),2);
        var_2 += pow(suma_2/3.0 - estado.distanceToGoal(mis_colores.at(1),i),2);
    }
    var_1 = sqrt(var_1/3); 
    var_2 = sqrt(var_2/3);

    double dato = pow(((pow(3*MAX_DIST- suma_1 - var_1/6,2) + pow(3*MAX_DIST - suma_2 - var_2/6,2))/2),1/2.0);

    return  dato; 

    

}

double FichasCasa(const Parchis &estado, int jugador, vector<color> &mis_colores){

    double suma = 0;

    for(int j = 0; j < 2; j++){
        suma += estado.piecesAtHome(mis_colores.at(j));
        if(estado.piecesAtHome(mis_colores.at(j)) == 3){
            suma+=2;
        }
    }
    

    return suma*10;
}

double Seguridad(const Parchis &estado, int jugador, vector<color> &mis_colores){
    double suma = 0; 

    for(int i = 0;i < 3; i++){
        for(int j = 0; j < 2; j++){
            if(estado.isSafePiece(mis_colores.at(j),i)){
                suma++;
            }
        }
    }

    return suma*10;
}
/*
double Captura(const Parchis &estado, int jugador, vector<color> &mis_colores){

    pair<color, int> aux = estado.eatenPiece();
    if(aux.first != none){

        for(int j = 0; j < 2; j++){
            if(aux.first == mis_colores.at(j)){
                return 0;
            }
        }

        return 20;
        
    }

    return 0;
    
}
*/
/*
double Barreras(const Parchis &estado, int jugador, vector<color> &mis_colores, vector<color> &otros_colores ){
    double valor = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            Box aux_box = estado.getBoard().getPiece(mis_colores.at(j),i).get_box();
            if(estado.isWall(aux_box) and aux_box.type != final_queue){
                valor  += 2;
                /*
                Box other_box((aux_box.num-10 +68)%68, aux_box.type, aux_box.col); 
                vector<pair <color, int>> aux = estado.allPiecesBetween(other_box, aux_box);

                for(auto it = aux.begin(); it != aux.end(); it++){
                    if((*it).first == otros_colores.at(0) or (*it).first == otros_colores.at(1)){
                        valor +=2; 
                    }
                }
                

            }
        }
    }
    return valor;
}
*/



double ValorJugador(const Parchis &estado, int jugador, vector<color> &mis_colores, vector<color> &otros_colores){
    double devolver = 0;
    devolver += 1.2*Distancia(estado,jugador, mis_colores); 
    devolver -= FichasCasa(estado,jugador, mis_colores); 
    devolver += Seguridad(estado,jugador, mis_colores); 

    
    if(estado.getAvailableNormalDices(jugador).size() == 1 and estado.getAvailableNormalDices(jugador).at(0) == 100){
        devolver += ValoracionDadoEspecial(estado.getPower(jugador),estado, jugador);
    }  
    

    if(estado.getPower(jugador) == 100){
        devolver -= 3*MAX_DIST;
    }

    if(estado.getPower(jugador) >= 50){
        devolver += 25;
    }



    for(int i = 0; i<2; i++){
        if(estado.piecesAtGoal(mis_colores.at(i)) == 2){
            for(int j = 0; j < 3; j++){
                if(estado.distanceToGoal(mis_colores.at(i),j) != 0){
                    devolver += MAX_DIST - estado.distanceToGoal(mis_colores.at(i),j);
                }
            }
            
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 2; k++){
                for(int l = 0; l< 3; l++){
                    int aux = estado.distanceBoxtoBox(mis_colores.at(i),j,otros_colores.at(k),l);
                    if(aux >= 0 and aux <= 7){
                        devolver += 2;
                    }
                }
            }
            
        }
    }
   
    

    return devolver; 

}



double AIPlayer::MiValoracion1(const Parchis &estado, int jugador){

    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }else{

        vector<color> mis_colores = estado.getPlayerColors(jugador); 
        vector<color> otros_colores = estado.getPlayerColors(oponente); 
        double valor_jugador = ValorJugador(estado,jugador, mis_colores,otros_colores);
        double valor_oponente = ValorJugador(estado,oponente, otros_colores,mis_colores);

        return valor_jugador - valor_oponente; 
    }

}


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}
