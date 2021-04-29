#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

#include "globals.h"
#include "memoria.cpp"
#include "riscv.cpp"

int main (int argc, const char * argv[]) {

    int n, opcao;
    char c = 1;
    init();     // inicia parametros globais
    load_mem("text.bin", 0);
    load_mem("data.bin", 0x2000);

    cout << "Qual opção deseja executar:\n"
         << "1-verificar instruções\n"
         << "2-rodar passo a passo (aperte enter para ir para o passo seguinte)\n"
         << "3-rodar programa até o final\n";
    cin >> opcao;

    switch(opcao){

        case 1:
            cout << "Numero de instrucoes a imprimir: ";
            cin >> n;
            for (int i=0; i < n; i++) {
                fetch();
                decode();
                cout << "Instr = " << instr_str[instruction]
                    <<  " Imm = " << imm32_t
                    << " rs1 = " << rs1
                    << " rs2 = " << rs2
                    << " rd = " << rd
                    << endl;
            }
            break;
        case 2:
            while((pc < DATA_SEGMENT_START) && !stop_prg){
                scanf("%c", &c);
                while(c != '\n');
                step();
                dump_breg();
                cout << "pc = " << pc << "\n";
                c = 0;
            }
            break;
        case 3:
            run();
            cout << "Deseja verificar o banco de registradores?\n1-sim\n2-não\n";
            cin >> opcao;
            if(opcao == 1){
                dump_breg();
            }
        break;
        default:
            cout << "Opção inválida\n";
            break;
    }

    return 0;
}
