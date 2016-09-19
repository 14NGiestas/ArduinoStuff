#include <Servo.h>
#include <NewPing.h>


#define TRIG_PIN   3
#define ECHO_PIN   4
#define MAX_DIST  50
#define VEL       10

//Servos Continuos: write define velocidade
Servo RodaE;
Servo RodaD;
//Micro Servos: write define angulo
Servo BaseSonar;
Servo RodaF;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DIST);

void setup()
{
    RodaE.attach(9);
    RodaD.attach(10);
    BaseSonar.attach(11);
    RodaF.attach(12);
}

void loop()
{

    int uS = sonar.ping();
    int DS = uS / US_ROUNDTRIP_CM;
    if (DS <= 5 && DS != 0)
    {
      //Deixa o robot parado
      RodaE.write(90);
      RodaD.write(90);

      int BestAngle = 0;
      int BestDist = 0;
      //Varre todo o angulo de visão do robot (180)
      for (int i = 0; i <= 180; i++)
      {
        //Posiciona o sonar nesse angulo
        BaseSonar.write(i);
        //Realiza a medida
        int uS = sonar.ping();
        int DS = uS / US_ROUNDTRIP_CM;
        delay(10);
        //Se a distancia for a maior ate agora
        if (DS > BestDist)
        {
          //entao eh a melhor distancia
          BestDist = DS;
          //e o melhor angulo
          BestAngle = i;
        }
      }
      //Posiciona o sonar para frente
      BaseSonar.write(90);
      //Recua o robo
      RodaE.write(0);
      RodaD.write(180);
      //Espera 1s para a manobra
      delay(1000);
      
      //Melhor angulo eh na Direita?
      if (BestAngle > 90)
      {
        //Roda da Frente vira 45 graus para a Direita
        RodaF.write(45);
        //Roda Esquerda em velocidade maxima para frente
        RodaE.write(180);
        //Roda Direita fica parada
        RodaD.write(90);
      }
      //Melhor angulo eh na Esquerda?
      else if (BestAngle <= 90)
      {
        //Roda da Frente vira 45 graus para a Esquerda
        RodaF.write(180 - 45);
        //Roda Esquerda fica parada
        RodaE.write(90);
        //Roda Direita em velocidade maxima para frente
        RodaD.write(0);
      }
      //Espera 1s para a manobra
      delay(1000);
      //Vira a roda da frente para a frente
      RodaF.write(90);
    }
    else
    {
      //ande para frente como se nao houvesse amanha
      RodaE.write(180 - VEL);
      RodaD.write(VEL);
    }
  }
}
