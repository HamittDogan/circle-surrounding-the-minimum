#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include<graphics.h>

int faktoriyel (int n)
{
    if(n==0)
        return 1;
    else
        return n*faktoriyel(n-1);
}
int kombinasyon(int n,int i)
{
    return(faktoriyel(n)/(faktoriyel(n-i)*faktoriyel(i)));
}


// noktayi temsil eden yapi
struct koordinat_noktalari
{
    float X, Y;
};


// daireyi temsil edecek yapi
struct daire
{
    koordinat_noktalari C;
    float R;
};


// iki nokta arasi uzakligi
// öklid yapisi ile bulan fonksiyon
float uzaklik(struct koordinat_noktalari &a, struct koordinat_noktalari &b)
{
    return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
}



// Asagidaki iki işlem, üç nokta verildiginde
// çemberin denklemini bulmak için kullanilan fonksiyonlardir.

// 3 nokta ile tanimlanan bir çemberi elde etmek için yardimci yöntem
koordinat_noktalari daire_merkez_bul(float bx, float by,
                                     float cx, float cy)
{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}

// 2 noktayla kesisen en küçük daireyi döndürme islevi
daire circle_from(struct koordinat_noktalari& A, struct koordinat_noktalari& B)
{
    // Merkezi, A ve B'nin orta noktasi olacak sekilde ayarlama
    koordinat_noktalari C = { (A.X + B.X) / 2.0, (A.Y + B.Y) / 2.0 };
    // Yaricapi AB mesafesinin yarisi olacak sekilde ayarlama
    return { C, uzaklik(A, B) / 2.0 };
}


// Üç noktayi da kesen bir daire döndürme islevi
daire circle_from2(struct koordinat_noktalari& A, struct koordinat_noktalari& B,
                   struct koordinat_noktalari& C)
{
    koordinat_noktalari I = daire_merkez_bul(B.X - A.X, B.Y - A.Y,
                            C.X - A.X, C.Y - A.Y);
    I.X += A.X;
    I.Y += A.Y;
    return { I, uzaklik(I, A) };
}


// bir noktanin içinde veya kenarinda
// olup olmadigini kontrol eden fonksiyon
bool icinde_mi(struct daire a, struct koordinat_noktalari p)
{
    if(uzaklik(a.C, p) <= a.R)
    {
        return true;
    }
    else
        return false;
}


// Bir dairenin verilen noktalari çevreledigini kontrol etme islevi
bool daire_gecerli_mi(struct daire  c, struct koordinat_noktalari * P)
{

    // Noktalarin çemberin içinde olup olmadigini
    // kontrol etmek için tüm noktalari tekrarlama
    for(int a = 0; a <sizeof(P); a++)
    {
        if (!icinde_mi(c, P[a]))
            return false;
    }
    return true;
}

// Döndürme işlevi, verilen nokta kümesinden
// minimum çevreleyen çemberi bulma
daire minimum_cevreleyen_daire(struct koordinat_noktalari* P)
{
    int n = sizeof(P);

    if (n == 0)
        return { { 0, 0 }, 0 };
    if (n == 1)
        return { P[0], 0 };

    // minimum çevreleyen daireyi cok büyük yaricapina sahip olacaka sekilde ayarlıyoruz.
    daire mcd = { { 0, 0 }, 1000 };

    // Tüm noktalari gözden geçirme
    for (int i = 0; i < n-1; i++)
    {
        for (int j = i+1; j < n; j++)
        {

            // P [i] ve P [j] ile kesisen en küçük daireyi alin
            daire gecici = circle_from(P[i], P[j]); //P nin elemanlari ile çemberler çiz orta naktalarini bul yaricapini bul

            // Tüm noktalari çevreleyen ve daha küçük bir yariçapa
            // sahip olan minimum çevreleyen çemberi güncelleme
            if (gecici.R < mcd.R && daire_gecerli_mi(gecici, P)) // buldugumuz cember tum noktalari kapsiyor mu
                mcd = gecici;
        }
    }

    // Tüm üç noktayi gözden geçirme
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j <n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {

                // P [i], P [j], P [k] ile kesisen daireyi bulma
                daire gecici = circle_from2(P[i], P[j], P[k]);

                // geçici tüm noktalari kapsiyorsa ve daha küçük bir yariçapina
                // sahipse minumum çevreleyen çemberi güncelleme
                if (gecici.R < mcd.R && daire_gecerli_mi(gecici, P))
                    mcd = gecici;
            }
        }
    }
    return mcd;
}



int main()
{
    int gd,gm;
    detectgraph(&gd,&gm);
    initwindow(600,600,"PROLAB PROJE",300,300);

    int piksel_kalinligi =25 ;
    int nokta_kalinligi = 3;
    int sizes=4;
    int sizes2=sizes+1;
    koordinat_noktalari degerler[sizes2] ;

    //noktalari txt dosyasidan alip diziye atama
    FILE *filep=fopen("degerler.txt","r");

    if(degerler==NULL)
    {

        printf("DOSYA YOK !");
    }
    int i=0;
    while(!feof(filep))
    {

        fscanf(filep,"%f %f",&degerler[i].X,&degerler[i].Y);

        i++;

    };
    fclose(filep);
//.txt den  aldigimiz noktalarin yerlerini degistiriyoruz.
    for(int s=0; s<2; s++)
    {
        int rastgele;
        rastgele=rand()%sizes2;
        struct koordinat_noktalari temp=degerler[sizes];
        degerler[sizes]=degerler[rastgele];
        degerler[rastgele]=temp;
    }
// noktalari yazdirma

    for(int i=0; i<sizes2; i++)
    {
        printf("%2.f %2.f \n",degerler[i].X,degerler[i].Y);
    }

    setcolor(14);
// koordinat düzlemi üstündeki kısa cizgileri cizdirme.

    for(int x=0; x<40; x++)
    {
        line(x*piksel_kalinligi,296,x*piksel_kalinligi,304);
    }


    setcolor(14);
    for(int x=0; x<40; x++)
    {
        line(296,x*piksel_kalinligi,304,x*piksel_kalinligi);
    }
  //  koordinat düzlemi cizdirme
    setcolor(14);
    line(0,300,600,300);
    line(300,0,300,600);

//dairenin ve noktalarin kalinligi
    setlinestyle(SOLID_LINE, 2, nokta_kalinligi );
    daire mcd = minimum_cevreleyen_daire(degerler);
    printf("Merkez:{%0.2f %0.2f}\nYaricap:%0.4f\n",mcd.C.X,mcd.C.Y, mcd.R);

    setcolor(15);
    char yazialani[100];
    snprintf(yazialani, sizeof(yazialani), "M=(%0.2f, %0.2f), R=%0.4f", mcd.C.X, mcd.C.Y, mcd.R);
    outtextxy(0, 0, yazialani);
    setcolor(5);



    mcd.C.X=(mcd.C.X)*piksel_kalinligi+300;
    mcd.C.Y=300-mcd.C.Y*piksel_kalinligi;
    mcd.R=mcd.R*piksel_kalinligi;
    setcolor(11);
    circle(mcd.C.X,mcd.C.Y,mcd.R);


    //nokta ve koordinatlarının işlevi

    for(int i=0; i<=sizes; i++)
    {
        float x = (degerler[i].X*piksel_kalinligi)+300;
        float y = (degerler[i].Y*-piksel_kalinligi)+300;


        //sayıların koordinatlarını konsola yazdırma
       char buffer[100];
        snprintf(buffer, sizeof(buffer), "(%d, %d)", (int)degerler[i].X, (int)degerler[i].Y);
        outtextxy(x, y, buffer);
        circle(x,y,nokta_kalinligi /2);
        setcolor(11);
    }


    koordinat_noktalari degerler_2[sizes2] ;
    FILE *fileps=fopen("degerler.txt","r");

    if(fileps==NULL)
    {
        printf("DOSYA YOK !");
    }
    int m=0;
    while(!feof(fileps))
    {

        fscanf(fileps,"%f %f",&degerler_2[m].X,&degerler_2[m].Y);

        m++;

    };
    fclose(fileps);

//selection sort
    int h,j;
    int minimum_index;
    for(h=0; h<sizes-1; h++)
    {
        minimum_index=h;
        for(j=1; j<sizes; j++)
        {
            if(degerler_2[j].X<degerler_2[minimum_index].X)
            {
                minimum_index=j;

            }
        }
        struct koordinat_noktalari temp=degerler_2[h];
        degerler_2[h]=degerler_2[minimum_index];
        degerler_2[minimum_index]=temp;
    }

    setcolor(11);
    int n=sizes;
    float t,x,y;

    delay(2000);
    for (t=0.0; t<=1.0; t+=0.0001)
    {
        x=0.0;
        y=0.0;
        for(i=0; i<=n; i++)
        {
            x=x+(kombinasyon(n,i)*pow(1-t,n-i)*pow(t,i)*degerler_2[i].X);
            y=y+(kombinasyon(n,i)*pow(1-t,n-i)*pow(t,i)*degerler_2[i].Y);
        }
        putpixel(x*piksel_kalinligi+300,300+y*(-piksel_kalinligi),1);
    }
    getch();
    return 0;
}
