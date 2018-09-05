/* 
 * File:   apagador.c
 * Author: Edgar
 *
 * Created on 14 de Dezembro de 2013, 14:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xc.h>

/* Palavra de Configuracao */
#pragma config CP = OFF, CPD = OFF, LVP = OFF, BOREN = OFF, PWRTE = OFF, WDTE = OFF, FOSC = HS
#define _XTAL_FREQ 8000000

int segundos;
int minutos;
int luz_acesa;

void repouso(void)
{
 RA0 = 0;/* Desliga Rele */
 PORTB = 0x1F;/* Liga LEDS do Contador e Desliga o LED da Luz */
 segundos=60;
 minutos=0;
 luz_acesa=0;
}

void mostra_minutos(void)
{
 int a;
 a=0;
 if ((segundos&1)==1) a=0x20;
 a=a+(minutos&0x1F);
 PORTB=a;
}

int chave_apertada(int n)
{
 int a;
 int cnt;
 int prossegue;
 a = PORTA&0x02;
 if (a==0x02) return(0);
 cnt=0;
 prossegue=1;
 while (prossegue)
   {
    a = PORTA&0x02;
    if (a==0x00)
      {
       __delay_ms(100);
       ++cnt;
       if (cnt==n) prossegue=0;
      }
    else prossegue=0;
   }
 if (cnt==n) return(1);
 return(0);
}

void main(int argc, char** argv)
{
 /* Configura portas */
 PCLATH = 0;
 INTCON = 0;
 PORTA = 0x00; /* Desliga o Rele e Deteccao da chave desligada */
 CMCON = 0x07; /* Desliga comparadores e habilita pinos de IO */
 TRISA = 0x02; /* Bit RA1 funciona como entrada de chave, Bit RA0 saida para Rele */
 PORTB = 0x20; /* Acende o LED de baixo para indicar ligado */
 TRISB = 0x00; /* Bits RB5 a RB0 tudo saida */
 repouso();
 /* Inicia Operacao */
 while (1)
   {
    if (luz_acesa==0)
      {
       repouso();
       if (chave_apertada(10))
         {
          luz_acesa=1;
          RA0=1;/* Liga Rele */
          minutos=20;
          segundos=60;
         }
      }
    if (luz_acesa==1)
      {
       if (chave_apertada(20)) luz_acesa=0;
       else
         {
          mostra_minutos();
          __delay_ms(1000);
          --segundos;
          if (segundos==0)
            {
             --minutos;
             segundos=60;
            }
          if ((segundos==60)&&(minutos==0)) luz_acesa=0;
         }
       }
    }
}
