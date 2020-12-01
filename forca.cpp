#include <iostream>
#include <string>
#include <stdlib.h>
#include <locale>
#include <time.h>

#define WORDSAMT 60 //quantidade de palavras
#define MAXATMP 5   //n�mero de chances

using namespace std;

//banco de palavras para singleplay
string words[WORDSAMT] = {
    "ABACAXI", "LARANJA", "MANGA", "ABACATE", "TANGERINA", "OVO", "FEIJOADA", "JABUTICABA",     //comida     [0]
    "CORINTHIAS", "PALMEIRAS", "CEARA", "SANTOS", "FLAMENGO",                                   //times      [1]
    "RELOGIO", "PULSEIRA", "COLAR", "CHAPEU", "SAPATO",                                         //acess�rios [2]
    "BRASIL", "JAPAO", "FINLANDIA", "AUSTRALIA", "CHINA", "PAQUISTAO", "PARAGUAI",              //pa�ses     [3]
    "GATO", "CACHORRO", "LEAO", "GIRAFA", "CAPIVARA", "ORANGOTANGO",                            //animais    [4]
    "VERMELHO", "AZUL", "ROSA", "VERDE", "MARROM", "PRETO",                                     //cores      [5]
    "HELICOPTERO", "CARRO", "AVIAO", "JETSKI", "CAMINHAO", "NAVIO",                             //ve�culos   [6]
    "SABONETE", "CONTROLE", "ALUMINIO", "ESTETOSCOPIO",                                         //objetos    [7]
    "ABRIL", "JULHO", "DEZEMBRO", "JANEIRO", "MAR�O",                                           //m�s        [8]
    "PIKACHU", "JOHNCENA", "NARUTO", "LEON", "CHARIZARD",                                       //personagem [9]
    "SILENTHILL", "GODOFWAR", "THELASTOFUS"                                                     //jogos      [10]
};

//declara��o de fun��es
void about                       ();    //tela "sobre" do jogo
int intro                        ();    //tela de introdu��o do jogo
void play        (bool multiplayer);    //fun��o principal do jogo
string randomWord(string *category);    //fun��o que sorteia palavra no singleplay
string masked         (string word);    //fun��o que converte a palavra em oculta
void showStatus (string word, int attempts, string letterlog, string msg, string hint); //fun��o que atualiza a tela

//in�cio do c�digo
//========================================================
int main(){
    //configs
    setlocale(LC_ALL,"");
    srand((unsigned) time (NULL));

    int option;

    do{ //menu inicial
        option = intro();

        switch(option){
            case 1: play(false);
                break;
            case 2: play(true);
                break;
            case 3: about();
                break;
        }

    }while(option < 1 || option > 4);

    return 0;
}

//-------------------------------------------------

int intro(){
    int opt;

    //menu
    cout << "Bem-Vindo ao Jogo da Forca 1.2\n";
    cout << "_______________________________\n\n";
    cout << "1 - JOGAR SOZINHO\n";
    cout << "2 - JOGAR EM DUPLA\n";
    cout << "3 - SOBRE\n";
    cout << "4 - SAIR \n";
    cout << "\nESCOLHA A OP��O DESEJADA\nE PRESSIONE ENTER\n";

    cin >> opt;

    system("CLS");
    return opt;
}

//-----------------------------------------------

void play(bool multiplayer){
    bool     failure;       //verifica se a letra digitada foi encontrada.
    bool  pastletter;       //verifica se a letra digitada j� havia sido digitada antes.
    char      letter;       //recebe a letra digitada pelo usu�rio.
    int     attempts;       //indica a quantidade de vezes que o jogador ainda pode fracassar.
    int  lettersleft;       //indica a quantidade de letras que faltam.
    string      word;       //recebe a palavra secreta.
    string letterlog;       //salva as letras j� digitadas
    string       msg;       //recebe a mensagem para o jogador
    string      hint;       //recebe a dica da palavra secreta

    //--------
    if(multiplayer){
        cout << "Desafiante, digite a palavra: ";
        cin >> word;
        cout << "\nDigite uma dica para o desafiado (sem espa�o): ";
        cin >> hint;
        cout << "\nDigite agora o n�mero de chances do desafiado: ";
        cin >> attempts;

        for(int i = 0; i < word.size(); i++){   //coloca palavra toda em mai�sculo
            word[i] = toupper(word[i]);
        }

        system("CLS");

    } else {
        word = randomWord(&hint);
        attempts = MAXATMP;
    }

    //-----------
    string maskedWord = masked(word);
    msg = "Bem-vindo ao jogo!";
    lettersleft = word.size();
    //-----------

    while(attempts > 0 && word != maskedWord){
        //reset de vari�veis de controle
        failure = true;
        pastletter = false;

        showStatus(maskedWord, attempts, letterlog, msg, hint); //atualiza tela

        //-----------------------------------
        if(lettersleft <= 3) cout << "Faltam 3 letras ou menos. Digite 1 para arriscar a palavra.\n";
        cout << "Digite uma letra: ";
        cin >> letter;

        //arriscar palavra
        if(letter == '1' && lettersleft <= 3){
            attempts = 0;
            cout << "\nDigite a palavra: ";
            cin >> maskedWord;

            system("CLS");

            for(int i = 0; i < maskedWord.size(); i++){
                maskedWord[i] = toupper(maskedWord[i]);
            }
        }
        //arriscar letra
        else {
            for(int i = 0; i < letterlog.size(); i++){  //procura no letterlog se letra j� foi digitada
                if(letterlog[i] == toupper(letter)){
                    msg = "Epa! Letra j� digitada";
                    failure = false;
                    pastletter = true;
                }
            }

            if(!pastletter){ //caso a letra n�o seja repetida

                lettersleft = 0;
                letterlog += toupper(letter);

                for(int i = 0; i < word.size(); i++){
                    if(word[i] == toupper(letter)){
                        failure = false;
                        maskedWord[i] = toupper(letter);
                        msg = "Acertou uma! Continue assim";
                    }

                    if(maskedWord[i] == '-') lettersleft++;     //� necess�rio a recontagem de letras
                            //pois o usu�rio pode acertar mais de 1 letra por vez,
                            //o que resultaria em erro na quantidade de letras que faltam
                }
            }

            system("CLS");

            if(failure){
                attempts--;
                msg = "OPS! N�o tem essa letra :C";
            }
        }
    } //fim do game

    int pontos = 0;

    //resultado
    if(word == maskedWord){
        pontos = 50 + (20 - letterlog.size());
        cout << "VOC� GANHOU!\n" << "quantidade de tentativas: " << letterlog.size()
        << "\npontos: " << pontos << "\n\n\n\n";
    } else {
        pontos = 20 - letterlog.size();
        cout << "VOC� PERDEU =C\n" << "pontos: " << pontos << "\n\n\n\n";
    }

    //-------- tela final -------------
    int opt = 0;
    cout << "Deseja reiniciar o jogo?\n";
    cout << "1 - SIM   2 - N�O\n";
    cin >> opt;

    if(opt == 1){
        system("CLS");
        main();
    } else {
        system("CLS");
        cout << "At� mais!\n\n\n\n\n\n";
        cout << "made by Emanoel Rodrigues - 2020\n\n\n";
    }

    system("PAUSE");
} //fim do jogo

//----------------------------------------

string masked(string word){
    string mask;

    for(int i = 0; i < word.size(); i++){
        mask += "-";
    }

    return mask;
}

//-----------------------------------------

void showStatus(string word, int attempts, string letterlog, string msg, string hint){
    cout << msg << "\n\n";
    cout << "Chances: " << attempts;
    cout << "\nLetras arriscadas: ";

    for(int i = 0; i < letterlog.size(); i++){
        cout << letterlog[i] << ", ";
    }

    cout << "\n\npalavra: " << word << "\n";
    cout << "DICA: " << hint << "\n\n";
}

//-----------------------------------------

void about(){
    cout << "Desenvolvido por Emanoel Rodrigues\n";
    cout << "Data da build: 01/12/2020\n\n\n";
    cout << "Curso: Linguagem C e C++ - Iniciante Ao Avan�ado por OneDay Code.\n\n";

    system("PAUSE");
    system("CLS");
    main();
}

//-----------------------------------------

string randomWord(string *category){
    int randomIndex = rand() % WORDSAMT;

    if     (randomIndex <= 7)  *category = "COMIDA";
    else if(randomIndex <= 12) *category = "TIME DE FUTEBOL";
    else if(randomIndex <= 17) *category = "ACESS�RIO VEST�VEL";
    else if(randomIndex <= 24) *category = "PA�S";
    else if(randomIndex <= 30) *category = "ANIMAL";
    else if(randomIndex <= 36) *category = "COR";
    else if(randomIndex <= 42) *category = "VE�CULO";
    else if(randomIndex <= 46) *category = "OBJETO";
    else if(randomIndex <= 51) *category = "M�S";
    else if(randomIndex <= 56) *category = "PERSONAGEM";
    else if(randomIndex <= 59) *category = "JOGO DE VIDEOGAME";

    return words[randomIndex];
}

//========================================================
