int v1 = 5;
int v2 = 13;
int j1 = 4;
int j2 = 12;
int r1 = 3;
int r2 = 11;

int vp1 = 10;
int vp2 = 2;
int rp1 = 9;
int rp2 = 14;

int p1 = 8;
int p2 = 15;

int c1 = 7;
int c2 = 16;

int ur1 = 17;
int lu1 = 6;

int ser = 21;
int ser_clk = 20;
int clk = 19;

int etat = 1;

bool rebond_ur1 = false;
bool rebond_lu1 = false;
bool urgence;
bool urgence_ur1 = false;
bool urgence_lu1 = false;

int avant_v1;
int avant_v2;

int valeur;
int tableau_chiffres [11][7] =
{
  {0, 1, 1, 1, 1, 1, 1}, // 0
  {0, 0, 0, 0, 1, 1, 0}, // 1
  {1, 0, 1, 1, 0, 1, 1}, // 2
  {1, 0, 0, 1, 1, 1, 1}, // 3
  {1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 1, 0, 1, 1, 0, 1}, // 5
  {1, 1, 1, 1, 1, 0, 1}, // 6
  {0, 0, 0, 0, 1, 1, 1}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 0, 1, 1, 1, 1}, // 9
  {0, 1, 1, 0, 1, 1, 0} // pause
};


void reset ()
{
  digitalWrite (v1, LOW);
  digitalWrite (v2, LOW);
  digitalWrite (j1, LOW);
  digitalWrite (j2, LOW);
  digitalWrite (r1, LOW);
  digitalWrite (r2, LOW);

  digitalWrite (vp1, LOW);
  digitalWrite (vp2, LOW);
  digitalWrite (rp1, LOW);
  digitalWrite (rp2, LOW);

  afficher (10);
}


// 2.5 - urgence de bouton
void etat_urgence ()
{
  if (!rebond_ur1)
  {
    rebond_ur1 = true;
    urgence_ur1 = !urgence_ur1;
    delay (100);
    rebond_ur1 = false;
  }
}


// 2.6 - urgence de nuit
void etat_luminosite ()
{
  if (!rebond_lu1)
  {
    rebond_lu1 = true;
    urgence_lu1 = !urgence_lu1;
    delay (100);
    rebond_lu1 = false;
  }
}


// 2.3 - boutons des piétons
void p1_presse ()
{
  if (9 <= etat <= 12)
  {
    etat = 13;
  }
}

void p2_presse ()
{
  if (2 <= etat <= 5)
  {
    etat = 6;
  }
}


// 3 - traduction de l'entier en 7 segments
void afficher (int entier)
{

  digitalWrite (ser, 0);

  // écriture
  digitalWrite (ser_clk, HIGH);
  digitalWrite (ser_clk, LOW);

  for (int segment = 0; segment < 7; segment ++)
  {
    // données
    valeur = tableau_chiffres[entier][segment];
    digitalWrite (ser, valeur);

    // écriture
    digitalWrite (ser_clk, HIGH);
    digitalWrite (ser_clk, LOW);
  }

  // envoi en sortie
  digitalWrite (clk, HIGH);
  digitalWrite (clk, LOW);
}


void setup ()
{
  Serial.begin (9600);

  pinMode (v1, OUTPUT);
  pinMode (v2, OUTPUT);
  pinMode (j1, OUTPUT);
  pinMode (j2, OUTPUT);
  pinMode (r1, OUTPUT);
  pinMode (r2, OUTPUT);

  pinMode (vp1, OUTPUT);
  pinMode (vp2, OUTPUT);
  pinMode (rp1, OUTPUT);
  pinMode (rp2, OUTPUT);

  pinMode (p1, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (p1), p1_presse, FALLING);
  pinMode (p2, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (p1), p2_presse, FALLING);
  
  pinMode (c1, INPUT_PULLUP);
  pinMode (c2, INPUT_PULLUP);
  
  pinMode (ur1, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (ur1), etat_urgence, RISING);
  pinMode (lu1, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt (lu1), etat_luminosite, CHANGE);

  pinMode (ser, OUTPUT);
  pinMode (ser_clk, OUTPUT);
  pinMode (clk, OUTPUT);
}


void loop ()
{
  
  reset ();


// 2.5 et 2.6 - urgence
  urgence = urgence_ur1 || urgence_lu1;
  
  while (urgence)
  {
    digitalWrite (j1, LOW);
    digitalWrite (j2, LOW);
    delay (500);
    
    digitalWrite (j1, HIGH);
    digitalWrite (j2, HIGH);
    delay (500);
  }


// 1.1 - gestion des feux routiers
  if (etat == 1)
  {
    digitalWrite (r1, HIGH);
    digitalWrite (r2, HIGH);
  }

  else if (2 <= etat && etat <= 6)
  {
    digitalWrite (r1, HIGH);
    digitalWrite (v2, HIGH);
  }

  else if (etat == 7)
  {
    digitalWrite (r1, HIGH);
    digitalWrite (j2, HIGH);
  }

  else if (etat == 8)
  {
    digitalWrite (r1, HIGH);
    digitalWrite (r2, HIGH);
  }

  else if (9 <= etat && etat <= 13)
  {
    digitalWrite (v1, HIGH);
    digitalWrite (r2, HIGH);
  }

  else if (etat == 14)
  {
    digitalWrite (j1, HIGH);
    digitalWrite (r2, HIGH);
  }


// 1.2 - gestion des feux piétons
  if (1 <= etat && etat <= 8)
  {
    digitalWrite (vp1, HIGH);
  }
  else
  {
    digitalWrite (rp1, HIGH);
  }

  if (etat == 1 || 8 <= etat && etat <= 14)
  {
    digitalWrite (vp2, HIGH);
  }
  else
  {
    digitalWrite (rp2, HIGH);
  }


// 2.4 - capteurs de présence
  switch (etat)
  {
    case 6:
    
      delay (1000);
      
      while (true)
      {
        if (digitalRead (c1))
        {
          break;
        }
      }
      break;

    case 13:
    
      delay (1000);
      
      while (true)
      {
        if (digitalRead (c2))
        {
          break;
        }
      }
      break;
  }


// 3 - affichage du temps restant
  avant_v1 = 9 - etat % 14;
  avant_v2 = 9 - (etat + 7) % 14;

  if (avant_v1 < 0) // cas où le feu 1 est déjà vert
  {
    avant_v1 = 0;
  }

  if (avant_v2 < 0) // cas où le feu 2 est déjà vert
  {
    avant_v2 = 0;
  }

  afficher (avant_v1);
  // afficher (avant_v2);


  delay (1000);
  etat %= 14;
  etat ++;
}
