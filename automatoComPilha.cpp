//Trabalho feito por Lucas Polidorio, Marcos Franca e Caua Monteiro
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <fstream>
using namespace std;

struct no{
    int idx_estado_adjacente;
    no* link = NULL;
    char cond_p_link;
    char link_empilha;
    char link_desempilha;
};

no* insereLista(no* l,int adjacente, char cond, char empilha, char desempilha);
char cin_verifica_alfabeto(string alfabeto);
vector<no*> inicializaGrafo(int n, string alfabeto);
void mostrarPilha(stack<char> pilha);
void criaDOT(const vector<no*> &estados, const string &nomeArquivo);

int main(){
    stack<char>pilha_automato;
    vector<no*>estados;
    int n, inicial, final;
    vector<int>estados_finais;
    string alfabeto, expressao;
    cout << "Entrada de alfabeto:" << endl;
    cin >> alfabeto;
    cout << "Entrada de num de nos:" << endl;
    cin >> n;
    cout << "Entrada do estado inicial e do numero de estados finais:" << endl;
    cin >> inicial >> final;
    cout << "de os estados finais" << endl;
    for(int i = 0 ; i < final ; i++)
    {
        int aux;
        while(cin >> aux && (aux >= n && aux < 0))
        {
            cout<< "Indice invalido" << endl;
        }
        estados_finais.push_back(aux);
    }
    estados = inicializaGrafo(n, alfabeto);

    criaDOT(estados, "automato.dot");

    cout << "Digite a expressao que voce quer verificar no automato" << endl;
    cin >> expressao;
    int posNo = inicial;
    for(int i = 0; i < expressao.length(); i++){
            while (estados[posNo] != NULL){
                
                cout << "Meu no e o " << posNo << endl;
                cout << "Leio " << expressao[i] << " Com i = " << i << endl;
                cout << "Verifico se " << estados[posNo]->cond_p_link << " e igual a " << expressao[i] << endl;
                if(estados[posNo]->cond_p_link == expressao[i]){
                    if(!pilha_automato.empty()){
                        if(estados[posNo]->link_desempilha != 126){
                            if(pilha_automato.top() == estados[posNo]->link_desempilha){
                                pilha_automato.pop();
                            }
                            else{
                                cout << "expressao invalida" << endl;
                                return -1;
                            }
                        }
                    }
                    if(estados[posNo]->link_empilha != 126){
                        pilha_automato.push(estados[posNo]->link_empilha);
                    }
                    
                    posNo = estados[posNo]->idx_estado_adjacente;
                    cout << "Troquei meu posNo para " << posNo << endl;
                    
                    break;
                }
                estados[posNo] = estados[posNo]->link;
                cout << "_______________________________________________"<< endl;
        
            }
        cout << endl << "Nostra pilha: ";
        mostrarPilha(pilha_automato);

    }
    bool cindicao = false;
    for(int i = 0; i<estados_finais.size() ;i++){
        if(estados_finais[i] == posNo){
            cindicao = true;
        }
    }

    if(cindicao && pilha_automato.empty())
    {
        cout << "Expressão aceita" << endl;
    }
    else{
        cout << "Expressão invalida" << endl;
    }

    system("dot -Tpng automato.dot -o automato.png");
    system("libreoffice automato.png");

    return 0;
}

vector<no*> inicializaGrafo(int n, string alfabeto)
{
    vector<no*>estados;
    no *p;
    int idx;
    char aux_cond, aux_empilha, aux_desempilha;
    for(int i = 0 ; i < n ; i++)
    {   
        cout << "No " << i << " se liga com quais nos? Informe tambem a condicao de transicao e o que e empilhado e desempilhado" << endl;

        p = NULL; 
        while(cin >> idx && idx != -1 && (idx < n && idx >= 0))
        {
            aux_cond = cin_verifica_alfabeto(alfabeto);
            aux_empilha = cin_verifica_alfabeto(alfabeto);
            aux_desempilha = cin_verifica_alfabeto(alfabeto);

            p = insereLista(p, idx, aux_cond, aux_empilha, aux_desempilha);
        }    
        estados.push_back(p);   
    }
    return estados;
}

no* insereLista(no* l,int adjacente, char cond, char empilha, char desempilha){
    no* n = new no;
    n->idx_estado_adjacente = adjacente;
    n->cond_p_link = cond;
    n->link_empilha = empilha;
    n->link_desempilha = desempilha;
    
    if(l == NULL){
        l = n;
    } else {
        no *P;
        P = l;
        while(P->link != NULL) {
            P = P->link;
        }
        P->link = n;
    }
    return l;
}

char cin_verifica_alfabeto(string alfabeto)
{
    int pos;
    char aux;
    cin >> aux;
    pos = alfabeto.find(aux);

    if(pos != -1)
    {
        return aux;
    }
    else{
        return 126;
        //§ é o nosso vazio
    }
}
void mostrarPilha(stack<char> pilha){
    while (!pilha.empty()) {
        cout << pilha.top() << " ";
        pilha.pop();
    }
    cout << endl;
}

void criaDOT(const vector<no*> &estados, const string &nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }
    arquivo << "digraph G {" << endl;
    for (int i = 0; i < estados.size(); i++) {
        no *atual = estados[i];
        while (atual != NULL) {
            arquivo << "    " << i << " -> " << atual->idx_estado_adjacente << " [label=\"" << atual->cond_p_link << "; " << atual->link_empilha << ", " << atual->link_desempilha << "\"];" << endl;
            atual = atual->link;
        }
    }
    arquivo << "}" << endl;
    arquivo.close();
}
