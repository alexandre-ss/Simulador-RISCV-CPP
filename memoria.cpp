#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

//Prototipos das funções a serem desenvolvidas
int32_t lw(uint32_t, int32_t);
int32_t lb(uint32_t, int32_t);
int32_t lbu(uint32_t, int32_t);
void sw(uint32_t, int32_t, int32_t);
void sb(uint32_t,int32_t, int8_t);

int32_t lw(uint32_t address, int32_t kte){
  uint32_t address_mem = address + kte;

  if(address_mem % 4 != 0){
    printf("O endereço não é um múltiplo de 4.\n");
    return 0;
  }
  address_mem /= 4;

  return mem[address_mem];

}

int32_t lb(uint32_t address, int32_t kte){
    if(address %4 != 0){
        printf("O endereço precisa ser um múltiplo de 4.\n");
        return 0;
    }
    uint32_t address_aux = address/4;
    int32_t aux = mem[address_aux];
    
    switch(kte){
        case 0:
                aux &= 0xFF;
                break;
        case 1:
                aux &= 0xFF00;
                break;                 
        case 2:
                aux &= 0xFF0000;
                break;
        case 3:
                aux &= 0xFF000000;
                break;
        default:
                if(kte > 3){
                    aux = lb(address + 4, (kte-4));
                    return aux;
                }
                else{return 0;}
                break;
    }
    aux = aux >> 8*kte;
    if(aux > 0x7F) aux |= 0xffffff00;
    return aux;
}

int32_t lbu(uint32_t address, int32_t kte){
    if(address%4 != 0){
        printf("O endereço precisa ser um múltiplo de 4.\n");
        return 0;
    }

    uint32_t address_aux = address/4;
    int32_t aux = mem[address_aux];

    switch(kte){
        case 0:
            aux &= 0xFF;
            break;
        case 1:
            aux &= 0xFF00;
            break;
        case 2:
            aux &= 0xFF0000;
            break;
        case 3:
            aux &= 0xFF000000;
            break;
        default:    
            if(kte > 3){
                aux = lbu(address + 4, (kte-4));
                return aux;
            } 
            else{return 0;}
            break;
    }   
    aux = aux >> 8*kte;
    return aux &= 0xFF;
}

void sw(uint32_t address, int32_t kte, int32_t data){
    int32_t address_mem = address + kte;

    if(address_mem % 4 != 0){
      printf("O endereço precisa ser um múltiplo de 4.\n");
    }else
    {
      address_mem /= 4;
      mem[address_mem] = data;
    }

}

void sb(uint32_t address, int32_t kte, int8_t data){
    if(address%4 != 0){
        printf("O endereço precisa ser um múltiplo de 4.\n");
    }
    else{             
        if(kte <= 3){
            address = address/4; 
            int32_t temp = data;
            temp = temp&0xFF;
            temp = temp << kte*8;
            switch(kte){
                case 0:
                    mem[address] &= 0xFFFFFF00; 
                    break;
                case 1:
                    mem[address] &= 0xFFFF00FF; 
                    break;
                case 2:
                    mem[address] &= 0xFF00FFFF; 
                    break;
                case 3:
                    mem[address] &= 0x00FFFFFF; 
                    break;
            }
            mem[address] = mem[address] | temp;
        }
        else{
            sb((address + 4), (kte-4), data);
        }
    }
}

