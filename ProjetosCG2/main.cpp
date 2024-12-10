#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#define ESC 27
#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>
GLuint idsTextura[1];

static int ombro = -30, cotovelo = 30, mao = -30, base = 0, medio = -45, indicador = -45, polegar = 20, rotacaoVertical = 0;

void carregarTextura(const char *nomeArquivo, int indice)
{
    int largura, altura, canais;
    unsigned char *dados = stbi_load(nomeArquivo, &largura, &altura, &canais, 0);

    if (!dados)
    {
        exit(1);
    }

    glGenTextures(1, &idsTextura[indice]);
    glBindTexture(GL_TEXTURE_2D, idsTextura[indice]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB, largura, altura, 0,
                 (canais == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dados);
    stbi_image_free(dados);
}

void reiniciarArticulacoes()
{
    ombro = -30;
    cotovelo = 30;
    mao = -30;
    base = 0;
    medio = -45;
    indicador = -45;
    polegar = 20;
    rotacaoVertical = 0;
    glutPostRedisplay();
}

bool iluminacaoAtiva = true; // Variável para controlar o estado da iluminação

void ajustarIluminacao()
{
    if (iluminacaoAtiva)
    {
        glDisable(GL_LIGHTING); // Desativa a iluminação
    }
    else
    {
        glEnable(GL_LIGHTING); // Ativa a iluminação
        GLfloat luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Luz ambiente (suave)
        GLfloat luzDifusa[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Luz branca
        GLfloat luzPosicao[] = {0.0f, 1.0f, 1.0f, 0.0f};  // Posição da luz

        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
        glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
        glEnable(GL_LIGHT0); // Ativa a luz GL_LIGHT0
    }

    iluminacaoAtiva = !iluminacaoAtiva; // Alterna o estado da iluminação
    glutPostRedisplay();                // Atualiza a cena
}

// Função para criar o menu
void createMenu()
{
    // Submenu de animação
    glutCreateMenu([](int option)
                          {
        if (option == 1) reiniciarArticulacoes();
        if (option == 2) ajustarIluminacao(); });

    glutAddMenuEntry("Reiniciar Articulacoes", 1);
    glutAddMenuEntry("Ajustar Iluminacao", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON); // Associa o menu ao botão direito do mouse
}

// Função para desenhar a base com textura
void desenhaBaseComTextura(GLuint *idsTextura) {
    GLUquadric *quadric;

    glPushMatrix();
    // Aplica a translação para posicionar a base
    glTranslatef(0.0, -2.0, 0.0);

    // Cria o objeto quadric para o cilindro
    quadric = gluNewQuadric();

    // Ativa o mapeamento de textura
    gluQuadricTexture(quadric, GL_TRUE);

    // Aplica a rotação para alinhar o cilindro com o eixo Y
    glRotatef(-90, 1, 0, 0);

    // Vincula a textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);

    // Desenha as laterais do cilindro
    gluCylinder(quadric, 0.8, 0.8, 1.2, 30, 30);

    // Desenha a base inferior
    glPushMatrix();
        glRotatef(180, 1, 0, 0); // Inverte para desenhar o disco na base
        gluDisk(quadric, 0.0, 0.8, 30, 1); // Raio interno 0.0, raio externo 0.8
    glPopMatrix();

    // Desenha o topo do cilindro
    glPushMatrix();
        glTranslatef(0.0, 0.0, 1.2); // Move para a posição do topo
        gluDisk(quadric, 0.0, 0.8, 30, 1);
    glPopMatrix();

    // Libera o objeto quadric
    gluDeleteQuadric(quadric);

    glPopMatrix();
}

// Função para desenhar o braço com textura
void desenhaBracoComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    glRotatef(90, 0, 0, 1);  // Rotação para alinhar o braço
    glScalef(2.5, 0.5, 0.5); // Escala para ajustar o tamanho do braço

    // Texturização na face frontal
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Texturização na face traseira
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glEnd();

    // Texturização na face superior
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Texturização na face inferior
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    // Texturização na face direita
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();

    // Texturização na face esquerda
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    glPopMatrix();
}

// Função para desenhar o antebraço com textura
void desenhaAntebraco(GLuint *idsTextura)
{
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    glScalef(2.0, 0.4, 0.4);

    // Texturização na face frontal
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.45, -0.45, 0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.45, -0.45, 0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.45, 0.45, 0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.45, 0.45, 0.45);
    glEnd();

    // Texturização na face traseira
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.45, 0.45, -0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.45, 0.45, -0.45);
    glEnd();

    // Texturização na face superior
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.45, 0.45, -0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.45, 0.45, -0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.45, 0.45, 0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.45, 0.45, 0.45);
    glEnd();

    // Texturização na face inferior
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.45, -0.45, 0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.45, -0.45, 0.45);
    glEnd();

    // Texturização na face direita
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.45, 0.45, -0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.45, 0.45, 0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.45, -0.45, 0.45);
    glEnd();

    // Texturização na face esquerda
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.45, -0.45, -0.45);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.45, 0.45, -0.45);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.45, 0.45, 0.45);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.45, -0.45, 0.45);
    glEnd();

    glPopMatrix();
}

// Função para desenhar a mão com textura
void desenhaMaoBracoComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    // Translação para posicionar a mão e o braço
    glTranslatef(0.0, -0.5, 0.0); // Translação no eixo Y
    glRotatef(90, 0, 0, 1);       // Rotação para alinhar o cubo (braço)
    glScalef(0.8, 0.3, 0.3);      // Escala do cubo para o tamanho desejado

    // Ativa a textura para o cubo
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);

    // Desenha o cubo com a textura
    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);

    // Face traseira
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);

    // Face superior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);

    // Face inferior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);

    // Face direita
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();

    glPopMatrix();
}

void desenhaCuboComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    // Aplica a textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]); // IDs de textura já carregados

    // Aumenta um pouco para dar forma ao cubo
    glScalef(1.04, 0.39, 0.65);

    // Desenha as faces do cubo com textura
    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);

    // Face traseira
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);

    // Face superior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);

    // Face inferior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);

    // Face direita
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();
    glPopMatrix();
}

void desenhaMedioComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    // Translada e rotaciona o medio
    glTranslatef(-0.3, 0.0, 0.2);
    glRotatef((GLfloat)medio, 0.0, 0.0, 1.0); // Rotação do medio
    glTranslatef(0.0, 0.6, 0.0);               // Posiciona o cubo do medio
    glRotatef(90, 0, 0, 1);                    // Rotaciona o cubo
    glScalef(2.5, 0.4, 0.4);                   // Escala do cubo

    // Aplica a textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[1]);

    // Desenha o cubo com a textura
    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face traseira
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, -0.25);

    // Face superior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face inferior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    // Face direita
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    glEnd();
    glPopMatrix();
}

void desenhaPolegarComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    // Translada e rotaciona o polegar
    glTranslatef(0.3, 0.0, 0.0);                // Posiciona o cubo do polegar
    glRotatef((GLfloat)polegar, 0.0, 0.0, 1.0); // Rotação do polegar
    glTranslatef(0.0, 0.6, 0.0);                // Posiciona o cubo para cima
    glRotatef(90, 0, 0, 1);                     // Rotaciona o cubo
    glScalef(2.5, 0.4, 0.4);                    // Escala do cubo

    // Aplica a textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[1]);

    // Desenha o cubo com a textura
    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face traseira
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, -0.25);

    // Face superior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face inferior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    // Face direita
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    glEnd();
    glPopMatrix();
}

void desenhaIndicadorComTextura(GLuint *idsTextura)
{
    glPushMatrix();
    // Translada e rotaciona o indicador
    glTranslatef(-0.3, 0.0, -0.2);                // Posiciona o cubo do indicador
    glRotatef((GLfloat)indicador, 0.0, 0.0, 1.0); // Rotação do indicador
    glTranslatef(0.0, 0.6, 0.0);                  // Posiciona o cubo para cima
    glRotatef(90, 0, 0, 1);                       // Rotaciona o cubo
    glScalef(2.5, 0.4, 0.4);                      // Escala do cubo

    // Aplica a textura
    glBindTexture(GL_TEXTURE_2D, idsTextura[1]);

    // Desenha o cubo com a textura
    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face traseira
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, -0.25);

    // Face superior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);

    // Face inferior
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    // Face direita
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.25, -0.2, 0.25);

    // Face esquerda
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.25, -0.2, -0.25);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-0.25, 0.2, -0.25);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-0.25, 0.2, 0.25);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.25, -0.2, 0.25);

    glEnd();
    glPopMatrix();
}

void init(void);
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);

// Fucao principal
int main(int argc, char **argv)
{
    // inicia o GLUT
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Braco robotico com articulacoes");

    init();

    // Criando o menu interativo
    createMenu();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}

// Funcao com alguns comandos para a inicializacao do OpenGL
void init(void)
{
    glClearColor(0.8, 0.8, 0.8, 1.0); // Define o fundo para cinza claro
    glEnable(GL_DEPTH_TEST);          // Habilita o algoritmo Z-Buffer

    glEnable(GL_TEXTURE_2D); // Habilita o uso de texturas

    // Carrega as texturas (verifique o caminho dos arquivos)
    carregarTextura("texturas/pele.jpeg", 0);    // Para o braço
    carregarTextura("texturas/peleDedo.jpg", 1); // Para o braço
}

// Funcal callback para o reshape da janela
void reshape(int w, int h)
{
    // muda para o modo GL_PROJECTION e reinicia a matriz de projecao
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // define o tamanho da area de desenho da janela
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Define a forma do volume de visualizacao para termos
    // uma projecao perspectiva (3D).
    gluPerspective(70, (float)w / (float)h, 1.0, 20.0); // Definindo o plano próximo como 1.0 e o distante como 20.0

    gluLookAt(2.0, 0.0, 8.0,  // posicao da camera (olho)
              -1.0, 0.0, 0.0, // direcao da camera (geralmente para centro da cena)
              0.0, 1.0, 0.0); // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode(GL_MODELVIEW);
}

// Fucao callback para controle das teclas comuns
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // Ombro (limite de -90 a +90 graus)
    case 'o':
        if (ombro > -90)
        {
            ombro -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'O':
        if (ombro < 90)
        {
            ombro += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    // Cotovelo (limite de 0 a 120 graus)
    case 'c':
        if (cotovelo > -90)
        {
            cotovelo -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'C':
        if (cotovelo < 90)
        {
            cotovelo += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    // Mão (limite de -90 a +90 graus)
    case 'm':
        if (mao > -90)
        {
            mao -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'M':
        if (mao < 90)
        {
            mao += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    case 'r':
        base = (base - 5) % 360;
        break; // sentido horário
    case 'R':
        base = (base + 5) % 360;
        break; // sentido anti-horário
    // Dedos (limite de -45 a +45 graus para cada dedo)
        case 'v':  // Rotação no eixo X (vertical)
        rotacaoVertical = (rotacaoVertical - 5) % 360;
        break;
    case 'V':  // Rotação no eixo X (vertical) no sentido contrário
        rotacaoVertical = (rotacaoVertical + 5) % 360;
        break;

    case 'd':
        if (indicador > -45)
        {
            indicador -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'D':
        if (indicador < 45)
        {
            indicador += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    case 'p':
        if (polegar > -45)
        {
            polegar -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'P':
        if (polegar < 45)
        {
            polegar += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    case 'i':
        if (medio > -45)
        {
            medio -= 5; // diminui a rotação, mas limita o intervalo
        }
        break; // sentido horário
    case 'I':
        if (medio < 45)
        {
            medio += 5; // aumenta a rotação, mas limita o intervalo
        }
        break; // sentido anti-horário

    // Fechar o programa
    case ESC:
        exit(EXIT_SUCCESS);
        break;
    }
    glutPostRedisplay();
}

// Funcao callback para desenhar na janela
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o Buffer de Cores
    glLoadIdentity();

    glPushMatrix();
    glRotatef(rotacaoVertical, 1.0, 0.0, 0.0);   // Rotação no eixo X (vertical)
    glRotatef((GLfloat)base, 0.0, 1.0, 0.0);
    // Base
    desenhaBaseComTextura(idsTextura);

    // Ombro
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(0.5, 30, 30); // raio, fatias, pilhas
    glPopMatrix();

    // Movimento do braco
    glPushMatrix();
    // origem posicionada no ombro
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)ombro, 0.0, 0.0, 1.0);
    // origem posicionada no centro do braco
    glTranslatef(0.0, 1.0, 0.0);

    // Braco com Textura
    desenhaBracoComTextura(idsTextura);

    // Cotovelo
    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(0.5, 30, 30); // raio, fatias, pilhas
    glPopMatrix();

    // Movimento do antebraco
    glPushMatrix();
    // origem posicionada no cotovelo
    glTranslatef(0.0, 1.0, 0.0);
    glutSolidSphere(0.4, 30, 30); // raio, fatias, pilhas
    glRotatef((GLfloat)cotovelo, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);

    desenhaAntebraco(idsTextura);

    // Movimento da mão
    glPushMatrix();
    // origem posicionada no abtebraço
    glTranslatef(0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, idsTextura[0]);
    glutSolidSphere(0.4, 30, 30); // raio, fatias, pilhas
    glRotatef((GLfloat)mao, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 1.0, 0.0);

    // mao-braco
    desenhaMaoBracoComTextura(idsTextura);

    // Movimento dos dedos
    desenhaCuboComTextura(idsTextura);

    // Medio
    desenhaMedioComTextura(idsTextura);

    // Polegar
    desenhaPolegarComTextura(idsTextura);

    // Indicador
    desenhaIndicadorComTextura(idsTextura);

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}