#include <InternetButton.h>
#include <math.h>

InternetButton b = InternetButton();

int xValue, yValue;
bool rollDice = false;
int tally []={0,0,0,0,0,0,0,0,0,0,0};

void setup() {
    b.begin();
    b.setBrightness(50);
    randomSeed(analogRead(0));
}

void loop() {
    
    yValue = b.readY(); //poll accelerometer for x accleration sideways)
    xValue = b.readX(); //poll accelerometer for x accleration sideways)
    
    if ((abs(xValue) > 50) or (abs(yValue) >50)) 
    {
        rollDice = true; //perform special check
        delay(20);
    }
    
    if (rollDice)
    {
        b.allLedsOff();
        //determine dice roll
        
        //flash all lights
        int rolls = random(1,5);
        
        
        for (int j = 0;j <rolls; j++)
        {
            for (int i = 1; i<12; i++)
            {
                b.ledOn(i,random(255),random(255),random(255));
                delay (50);
            }
            for (int i = 11; i>0; i--)
            {
                b.ledOff(i);
                delay (50);
            }
        }
        
        int die1 = floor(random(1,7));
        int die2 = floor(random(1,7));
        int total = die1+die2;
        
        int rcol, gcol, bcol = 0;
        
        //slow flashing to result
        for (int i = 1; i<(total+1); i++)
        {
            if (i != 12)
            {
                bcol = 255;
                if (i>3)
                {
                    gcol = 255;
                    bcol = 0;
                }
                if (i > 6)
                {
                    gcol = 0;
                    bcol = 255;
                }
                if (i > 9)
                {
                    gcol = 255;
                    bcol = 0;
                }
                
                b.ledOn(i,rcol,gcol,bcol);
                //delay (60+(20*i));
                delay (500-(20*(13-i)));
            }
        }
        
        if (total == 7)
        {
            for (int i = 0; (i<total+1); i++)
            {
                b.ledOn(i,255,0,0);
            }
        }
        if (total == 12)
        {
            b.ledOn(11,255,0,0);
        }
        
        //Particle.publish("rolls", String(rolls));
        //delay(50);
        //Particle.publish("total", String(total));
        
        tally[total-2] ++;
        
        rollDice = false;
    }
    
    if(b.buttonOn(3))
    {
        String textTally = "";
        for (int i = 0; i<11; i++)
        {
            textTally = textTally + String(tally[i])+",";
        }
        Particle.publish("tallies", textTally);
        delay (200);
    }
}
