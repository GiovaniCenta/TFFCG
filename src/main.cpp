//#include "camera_posize.cpp"
#include "loadFunctions.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

//LIMITES DO MAPA


#define LIMITEHORIZONTAL 5.0
#define LIMITEVERTICAL 3.0
#define LIMITEFRENTEATRAS 5.0
#define SKYBOXTAM 5.0


//VELOCIDADE DAS ESFERAS
#define VESFERASPADRAO 0.05
#define VESFERASHARD 0.2
#define VESFERASEASY 0.01

//ESSAS VELOCIDADES SERÃO DEFINIDAS POR ESTES BOOLEANOS
bool pressRightCtrl = false;
bool pressRightShift = false;
bool pressRightAlt = true;


//Definição das escalas(tamanho multiplicado por) dos coelhos e das esferas
#define ESCALA_COELHOS 0.2
#define ESCALA_ESFERA 0.27 //RAIO DA ESFERA

#define LARGURA_VACA 0.10

//aqui eu defino a posição na qual a camera ficara atras da vaca, isso tem que ser ajustado nas colisões
#define AJUSTE_VACA 0.3

//um ajuste da camera para a colisão que considera a distancia que fica o pescoco da vaca
#define PESCOCO_VACA 0.15

#define ALTURA_PADRAO_CAMERA_3P 0.0
#define ALTURA_PADRAO_CAMERA_ISO 0
#define POSICAO_INICIAL_CAMERA_Z 4

//DEFINICAO DA CAMERA
bool pressX = false;
bool pressC = false;


int direcaoe1 = 1;
int direcao = 1;
int direcaoe2 = 1;
int direcaoe3 = 1;
int direcaoe4 = 1;
int direcaoe5 = 1;
int direcaoe6 = 1;
int direcaoe7 = 1;
int direcaoe8 = 1;
int direcaoe9 = 1;
int direcaoe10 = 1;

int direcaoCoelho1 = 1;



float posicaoYcoelho;


float posicao_coelho_X;
float posicaoYcoelho1 = 0;
float posicao_coelho_Z = -3;




float posicaoXesfera;
float posicao_esfera_x;
float posicao_esfera_x2 ;
float posicao_esfera_x3;
float posicao_esfera_x4;
float posicao_esfera_x5;
float posicao_esfera_x6;
float posicao_esfera_x7 ;
float posicao_esfera_x8;
float posicao_esfera_x9;
float posicao_esfera_x10;
int flag = 1;


#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"



//void TextRendering_TextInfo(GLFWwindow* window, bool venceu);
void TextRendering_ShowText(GLFWwindow* window, int resultado);
void TextRendering_ShowTime(GLFWwindow* window, int resultado);



bool Colisao(float cx,  float cz, float sx, float sz);
void carregaObjetos(int argc, char* argv[]);
void desenhaObjetos();
void teste();
bool colisao(SceneObject a, SceneObject b, glm::mat4 modelA, glm::mat4 modelB);

int movimenta_esfera(int direcaoe4,float vel,float &posicaoXesfera,float posicao_esfera_Z,glm::mat4 model_esfera);
int movimenta_coelhos(int direc,float vel,float& posicaoYcoelho,float posicao_coelho_X,float posicao_coelho_Z,glm::mat4 modelesf);

float dificuldade(GLFWwindow* window,bool& pressRightCtrl, bool& pressRightShift, bool& pressRightAlt);

;

glm::vec4 colisao_parede(GLFWwindow* window,glm::vec4 camera_position_c);
glm::vec4 colisao_esfera(GLFWwindow* window,glm::vec4 camera_position_c);
glm::vec4 colisao_coelho(GLFWwindow* window,glm::vec4 camera_position_c);




void randomiza_posicao_coelho(float& posicao_coelho_X )
{
    posicao_coelho_X = rand() % 4;

}


void randomiza_posicao_camera_x(float& posicao_camera_X)
{
    posicao_camera_X = rand() % 4;

}
//random_x_coelho = rand() % 4;
float timeprev = glfwGetTime();
bool look_camera = true;


int resultado = 0;



//Posicao inicial da fps camera
glm::vec4 posicao_inicial_camera = glm::vec4(0.0,ALTURA_PADRAO_CAMERA_3P,POSICAO_INICIAL_CAMERA_Z,1.0f);


int main(int argc, char* argv[])
{




//int random_z_coelho = rand() % -6 + 4;

    randomiza_posicao_coelho(posicao_coelho_X);
    printf("\nx do coelho = %d", posicao_coelho_X);



    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }


    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "TRABALHO FINAL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);

    glfwSetScrollCallback(window, ScrollCallback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600);
    LoadShadersFromFiles();


    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/tetomadeira.jpg");      // TextureImage0
    LoadTextureImage("../../data/esferamadeira.jpg"); // TextureImage1
    LoadTextureImage("../../data/madeira.jpg");   //TextureImage2
    LoadTextureImage("../../data/tetomadeira.jpg"); //TextureImage3
    LoadTextureImage("../../data/coelho2.jpg"); //TextureImage4

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ObjModel spheremodel("../../data/sphere.obj");
    ComputeNormals(&spheremodel);
    BuildTrianglesAndAddToVirtualScene(&spheremodel);

    ObjModel spheremodel1("../../data/sphere.obj");
    ComputeNormals(&spheremodel1);
    BuildTrianglesAndAddToVirtualScene(&spheremodel1);

    ObjModel spheremodel0("../../data/sphere.obj");
    ComputeNormals(&spheremodel0);
    BuildTrianglesAndAddToVirtualScene(&spheremodel0);

    ObjModel spheremodel3("../../data/sphere.obj");
    ComputeNormals(&spheremodel3);
    BuildTrianglesAndAddToVirtualScene(&spheremodel3);

    ObjModel manmodel("../../data/a.obj");
    ComputeNormals(&manmodel);
    BuildTrianglesAndAddToVirtualScene(&manmodel);



    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

    ObjModel spheremodel2("../../data/cow.obj");
    ComputeNormals(&spheremodel2);
    BuildTrianglesAndAddToVirtualScene(&spheremodel2);

    ObjModel cube("../../data/RubixCube.obj");
    ComputeNormals(&cube);
    BuildTrianglesAndAddToVirtualScene(&cube);




    ObjModel bunny("../../data/bunny.obj");
    ComputeNormals(&bunny);
    BuildTrianglesAndAddToVirtualScene(&bunny);




    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }


    TextRendering_Init();
    glEnable(GL_DEPTH_TEST);
    glm::mat4 the_projection;
    glm::mat4 the_model;
    glm::mat4 the_view;


    //posicao inicial da camera

    glm::vec4 camera_position_c  = posicao_inicial_camera;


    // Ficamos em loop, renderizando, até que o usuário feche a janela

    direcao=1;



    randomiza_posicao_camera_x(camera_position_c.x );
    randomiza_posicao_coelho(posicao_coelho_X);
    while (!glfwWindowShouldClose(window))
    {
    resultado = 0;





        // Aqui executamos as operações de renderização



        float timenow = glfwGetTime();
        float deltatime = timenow - timeprev;
        timeprev = glfwGetTime();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_id);

        float r = g_CameraDistance;
        float y = r*sin(g_CameraPhi);
        float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
        float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);


        glm::vec4 camera_lookat_l    = glm::vec4(0.0f,1.0f,0.0f,1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
        glm::vec4 camera_view_vector = glm::vec4(-x, -y, -z, 0.0);
        glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0,0.0f,0.0f); // Vetor "up"



        //minha fpsCamera

        camera_position_c = fpsCamera(window,camera_position_c,camera_view_vector,camera_up_vector);



        glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

        // Agora computamos a matriz de Projeção.
        glm::mat4 projection;


        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -200.0f; // Posição do "far plane"

        if (g_UsePerspectiveProjection)
        {

            float field_of_view = 3.141592 / 1.8f;
            projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        }
        else
        {

            float t = 1.5f*g_CameraDistance/2.5f;
            float b = -t;
            float r = t*g_ScreenRatio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
            glm::mat4 view = Matrix_Camera_View(camera_lookat_l, camera_view_vector, camera_up_vector);
        }




        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));




        {

#define SPHERE 0
#define BUNNY  1
#define CHAO  2
#define HOMEM 3
#define SPHERE2 4
#define SKY_BOX 5
#define CEU 6
#define CUBE 7
#define SKY_BOX_0 8
#define SKY_BOX_1 9
#define SKY_BOX_2 10
#define SKY_BOX_3 11

            glm::mat4 model_esfera = Matrix_Identity();



            glm::mat4 model_bunny = Matrix_Identity();
            glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem
            // Desenhamos o modelo da esfera

            //Chamar a funcao de dificuldade, que aumenta a velocidade das esferas
            float velocidadedeMovimento = VESFERASPADRAO;
            velocidadedeMovimento = dificuldade(window,pressRightCtrl,pressRightShift,pressRightAlt);





     //================================================================================================
            //DESENHOS E COLISÕES
            direcaoe1 = movimenta_esfera(direcaoe1,velocidadedeMovimento,posicao_esfera_x, -2,model_esfera);

            //printf("posicao_esfera1_X: %.2f\n posicao_camera_x: %.2f",posicao_esfera_x, camera_position_c.x );

            if(	(camera_position_c.z >= -2 - ESCALA_ESFERA - AJUSTE_VACA - PESCOCO_VACA  &&
                    camera_position_c.z <= -2 + ESCALA_ESFERA + AJUSTE_VACA + PESCOCO_VACA) &&
                    (posicao_esfera_x - ESCALA_ESFERA - LARGURA_VACA<= camera_position_c.x &&
                     posicao_esfera_x + ESCALA_ESFERA + LARGURA_VACA >= camera_position_c.x))
            {
                printf("\n\n\n\n\n colisao 1");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);

            }





            direcaoe2 = movimenta_esfera(direcaoe2,velocidadedeMovimento * 2,posicao_esfera_x2, -1,model_esfera);


            if(	(camera_position_c.z >= -1 - ESCALA_ESFERA - AJUSTE_VACA - PESCOCO_VACA  &&
                    camera_position_c.z <= -1 + ESCALA_ESFERA + AJUSTE_VACA + PESCOCO_VACA ) &&
                    (posicao_esfera_x2 - ESCALA_ESFERA<= camera_position_c.x &&
                     posicao_esfera_x2 + ESCALA_ESFERA >= camera_position_c.x))
            {
                printf("\n\n\n\n\n colisao 2");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);

            }


            direcaoe3 = movimenta_esfera(direcaoe3,velocidadedeMovimento/2,posicao_esfera_x3, 0,model_esfera);

            if(	(camera_position_c.z >= 0 - ESCALA_ESFERA -AJUSTE_VACA- PESCOCO_VACA  &&
                    camera_position_c.z <= 0 + ESCALA_ESFERA +AJUSTE_VACA+ PESCOCO_VACA) &&
                    (posicao_esfera_x3 - ESCALA_ESFERA - LARGURA_VACA <= camera_position_c.x  &&
                     posicao_esfera_x3 + ESCALA_ESFERA + LARGURA_VACA>= camera_position_c.x))
            {

                printf("\n\n\n\n\n colisao 3");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);
            }
            direcaoe4 = movimenta_esfera(direcaoe4,velocidadedeMovimento * 3,posicao_esfera_x4, 1,model_esfera);

            if(	(camera_position_c.z >= 1 - ESCALA_ESFERA - AJUSTE_VACA - PESCOCO_VACA  &&
                    camera_position_c.z <= 1 + ESCALA_ESFERA + AJUSTE_VACA + PESCOCO_VACA ) &&
                    (posicao_esfera_x4 - ESCALA_ESFERA - LARGURA_VACA<= camera_position_c.x  &&
                     posicao_esfera_x4 + ESCALA_ESFERA + LARGURA_VACA>= camera_position_c.x))
            {
                printf("\n\n\n\n\n colisao 4");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);
            }
            direcaoe5 = movimenta_esfera(direcaoe5,velocidadedeMovimento/2,posicao_esfera_x5, 2,model_esfera);

            if(	(camera_position_c.z >= 2 - ESCALA_ESFERA -AJUSTE_VACA - PESCOCO_VACA  &&
                    camera_position_c.z <= 2 + ESCALA_ESFERA + AJUSTE_VACA + PESCOCO_VACA ) &&
                    (posicao_esfera_x5 - ESCALA_ESFERA - LARGURA_VACA<= camera_position_c.x  &&
                     posicao_esfera_x5 + ESCALA_ESFERA + LARGURA_VACA>= camera_position_c.x))
            {
                printf("\n\n\n\n\n colisao 5");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);
            }
            direcaoe6 = movimenta_esfera(direcaoe6,velocidadedeMovimento,posicao_esfera_x6, 3,model_esfera);

            if(	(camera_position_c.z >= 3 - ESCALA_ESFERA -AJUSTE_VACA - PESCOCO_VACA  &&
                    camera_position_c.z <= 3 + ESCALA_ESFERA +AJUSTE_VACA+ PESCOCO_VACA ) &&
                    (posicao_esfera_x6 - ESCALA_ESFERA - LARGURA_VACA<= camera_position_c.x  &&
                     posicao_esfera_x6 + ESCALA_ESFERA + LARGURA_VACA>= camera_position_c.x))
            {
                printf("\n\n\n\n\n colisao 6");
                camera_position_c = colisao_esfera(window,camera_position_c);
                resultado = 1;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);
            }

            /*

            if(    (camera_position_c.z <= -2 - ESCALA_ESFERA  &&
                        camera_position_c.z <= -2 + ESCALA_ESFERA ) &&
                        (posicao_esfera_x - ESCALA_ESFERA<= camera_position_c.x  &&
                         posicao_esfera_x + ESCALA_ESFERA >= camera_position_c.x))
                       {

                        printf("\n\n\n\n\n colisao 1");



            }*/


            direcaoCoelho1 = movimenta_coelhos(direcaoCoelho1, velocidadedeMovimento/2,posicaoYcoelho1,posicao_coelho_X,posicao_coelho_Z,model_bunny);
            if(	(camera_position_c.z >= posicao_coelho_Z - ESCALA_COELHOS - AJUSTE_VACA - PESCOCO_VACA &&
                    camera_position_c.z <= posicao_coelho_Z + ESCALA_COELHOS + AJUSTE_VACA + PESCOCO_VACA) &&
                    (posicao_coelho_X - ESCALA_COELHOS - LARGURA_VACA<= camera_position_c.x  &&
                     posicao_coelho_X + ESCALA_COELHOS + LARGURA_VACA >= camera_position_c.x))
            {


                //printf("\n\n\n\n\n colisao coelho");
                camera_position_c = colisao_coelho(window,camera_position_c);
                resultado = 2;
                randomiza_posicao_coelho(posicao_coelho_X);
                randomiza_posicao_camera_x(camera_position_c.x);
            }



            if(	(camera_position_c.z <= -LIMITEFRENTEATRAS - AJUSTE_VACA + 0.82 ||
                    camera_position_c.z >= LIMITEFRENTEATRAS + AJUSTE_VACA  - 0.3))
            {
                printf("\n\n\n COLISAO COM A PAREDE");
                camera_position_c = colisao_parede(window,camera_position_c);
            }

            if(	(camera_position_c.x <= -LIMITEHORIZONTAL + LARGURA_VACA  ||
                    camera_position_c.x >= LIMITEHORIZONTAL - LARGURA_VACA ))
            {
                printf("\n\n\n COLISAO COM A PAREDE");
                camera_position_c = colisao_parede(window,camera_position_c);
            }

//================================================================================================



















        // VACA (o player, acompanha a camera)



            glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
            model =Matrix_Translate(camera_position_c.x,-0.25,camera_position_c.z-AJUSTE_VACA);

            model = model *Matrix_Scale(0.25f,0.25f,0.25f) * Matrix_Rotate_Y(M_PI/2);

            /*if (g_LastCursorPosX/g_LastCursorPosY > 0 && g_LastCursorPosX/g_LastCursorPosY < M_PI/2 )
            model = model *Matrix_Rotate_Y(g_LastCursorPosX*-0.01);
            */

            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, HOMEM);
            DrawVirtualObject("cow");




            //LIMITES DO MAPA


            // Desenhamos o plano do chão
            model = Matrix_Translate(0.0f,-0.5f,0.0f);
            model = model * Matrix_Scale(SKYBOXTAM, 0.0f, SKYBOXTAM);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, CHAO);
            DrawVirtualObject("plane");


#define AJUSTE 2

            //SKY BOX LATERAL ESQUERDA
            model = Matrix_Translate(-LIMITEHORIZONTAL,2.5f,0.0f) * Matrix_Scale(0.0f, SKYBOXTAM -AJUSTE, SKYBOXTAM)
                    *Matrix_Rotate_Z(M_PI_2) ;
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, SKY_BOX_0);
            DrawVirtualObject("plane");



            //SKY BOX LATERAL DIREITA
            model = Matrix_Translate(LIMITEHORIZONTAL,2.5f,0.0f) * Matrix_Scale(0.0f, SKYBOXTAM-AJUSTE, SKYBOXTAM)
                    *Matrix_Rotate_Z(M_PI_2);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, SKY_BOX_1);
            DrawVirtualObject("plane");

            //SKY BOX SUPERIOR
            model = Matrix_Translate(0.0f,LIMITEVERTICAL,0.0f) * Matrix_Scale(SKYBOXTAM, 0.0f, SKYBOXTAM);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, CEU);
            DrawVirtualObject("plane");

            //SKY BOX FRONTAL
            model = Matrix_Translate(0.0f,2.5f,LIMITEFRENTEATRAS) * Matrix_Scale(SKYBOXTAM, SKYBOXTAM-AJUSTE,0.0f )
                    *Matrix_Rotate_X(M_PI_2);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, SKY_BOX_2);
            DrawVirtualObject("plane");

            //SKY BOX FRONTAL
            model = Matrix_Translate(0.0f,2.5f,-LIMITEFRENTEATRAS) * Matrix_Scale(SKYBOXTAM, SKYBOXTAM-AJUSTE,0.0f )
                    *Matrix_Rotate_X(M_PI_2);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(object_id_uniform, SKY_BOX_3);
            DrawVirtualObject("plane");






            //TextRendering_ShowEulerAngles(window);
            TextRendering_ShowProjection(window);
            TextRendering_ShowFramesPerSecond(window);

            TextRendering_ShowText(window,resultado);
            TextRendering_ShowTime(window,resultado);






            glfwSwapBuffers(window);


            glfwPollEvents();
        }
    }
// Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

// Fim do programa

    return 0;

}







glm::vec4 fpsCamera(GLFWwindow* window,glm::vec4 camera_position_c,glm::vec4 camera_view_vector,glm::vec4 camera_up_vector)
{
    glm::vec4 w = -camera_view_vector;
    glm::vec4 u =  crossproduct(camera_up_vector,w) /* PREENCHA AQUI o cálculo do vetor u */;
    // Normalizamos os vetores u e w
    w = w / norm(w);
    u = u / norm(u);

    glm::vec4 v = crossproduct(w,u);

    float speed = 0.02f;
    int stateW = glfwGetKey(window, GLFW_KEY_W);
    int stateD = glfwGetKey(window, GLFW_KEY_D);
    int stateS = glfwGetKey(window, GLFW_KEY_S);
    int stateA = glfwGetKey(window, GLFW_KEY_A);
    int stateSHIFT = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT);
    int stateSPACE = glfwGetKey(window,GLFW_KEY_SPACE);
    int stateCTRL = glfwGetKey(window,GLFW_KEY_LEFT_CONTROL);

     int stateX = glfwGetKey(window, GLFW_KEY_X);
    int stateC = glfwGetKey(window, GLFW_KEY_C);



    //Mecanica de corrida, segurou shift anda 2 vezes mais rapido
    if (stateSHIFT == GLFW_PRESS)
    {
        speed = speed * 2;
    }


    if (stateW == GLFW_PRESS)
    {
        //printf("\n%f", camera_position_c.x);
        camera_position_c = camera_position_c + (-w * speed);

    }
    if (stateS == GLFW_PRESS)
    {
        camera_position_c = camera_position_c + (w * speed);
    }
    if (stateA == GLFW_PRESS)
    {
        camera_position_c = camera_position_c + (-u * speed);
    }
    if (stateD == GLFW_PRESS)
    {
        camera_position_c = camera_position_c + (u * speed);
    }


    int i = 0;




    //Mecanica de pulo, sobre umas posicoes em y
    //tem que arrumar pq se segura fica no ar

if (stateX == GLFW_PRESS)
    {
        pressX = true;
        pressC = false;
    }
if (stateC = GLFW_PRESS)
{

        pressC = true;
        pressX = false;
}
if (pressC){

        //printf("\nentrou aqui");


        if (stateSPACE == GLFW_PRESS)
        {


            camera_position_c.y = ALTURA_PADRAO_CAMERA_ISO+ 0.1;



        }

    //mecanica de agachamento, mesmo problema
        else if (stateCTRL == GLFW_PRESS)
        {


            camera_position_c.y = ALTURA_PADRAO_CAMERA_ISO -0.1;

        }


        else
        {
            camera_position_c.y = ALTURA_PADRAO_CAMERA_ISO;
        }

}






if (pressX){


        if (stateSPACE == GLFW_PRESS)
        {


            camera_position_c.y = ALTURA_PADRAO_CAMERA_3P+ 0.1;



        }

    //mecanica de agachamento, mesmo problema
        else if (stateCTRL == GLFW_PRESS)
        {


            camera_position_c.y = ALTURA_PADRAO_CAMERA_3P -0.1;

        }


        else
        {
            camera_position_c.y = ALTURA_PADRAO_CAMERA_3P;
        }

}


    return camera_position_c;
}






//Funcao pra fazer as esferas e movimentar elas em direção as paredes
int movimenta_esfera(int direc,float vel,float& posicaoXesfera,float posicao_esfera_Z,glm::mat4 modelesf)
{

    if ((direc == 1))
    {
        if(posicaoXesfera >= LIMITEHORIZONTAL)
        {
            direc = 0;
            posicaoXesfera = (posicaoXesfera - vel) ;

        }
        else
        {

            posicaoXesfera = (posicaoXesfera + vel) ;

        }
        //printf("Posicao esfera indo pra direita: %.2f \n",posicao_esfera_x);
    }
    else
    {
        if(posicaoXesfera <= -LIMITEHORIZONTAL)
        {
            direc = 1;
            posicaoXesfera = posicaoXesfera + vel ;

        }
        else
        {
            posicaoXesfera = (posicaoXesfera - vel ) ;

        }
    }


    modelesf = Matrix_Translate(posicaoXesfera,-0.2f,posicao_esfera_Z) * Matrix_Scale(0.2f, 0.2f, 0.2f)
               * Matrix_Rotate_Z(g_AngleZ + (float)glfwGetTime() * 0.2f)
               * Matrix_Rotate_X(g_AngleX + (float)glfwGetTime() * 0.4f)
               * Matrix_Rotate_Y(g_AngleY + (float)glfwGetTime() * 0.1f);



    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(modelesf));
    glUniform1i(object_id_uniform, SPHERE);
    DrawVirtualObject("sphere");
    return direc;




}

//Funcao pra criar o coelho
int movimenta_coelhos(int direc,float vel,float& posicaoYcoelho,float posicao_coelho_X,float posicao_coelho_Z,glm::mat4 modelbunny)
{

    if ((direc == 1))
    {
        if( posicaoYcoelho >= 5)
        {
            direc = 0;
            posicaoYcoelho = ( posicaoYcoelho - vel) ;


        }
        else
        {

            posicaoYcoelho = ( posicaoYcoelho + vel) ;


        }

    }
    else
    {
        if( posicaoYcoelho <= 0)
        {
            direc = 1;
            posicaoYcoelho =  posicaoYcoelho + vel ;


        }
        else
        {
            posicaoYcoelho = ( posicaoYcoelho - vel ) ;


        }
    }


    modelbunny = Matrix_Translate(posicao_coelho_X,0.0 - ESCALA_COELHOS,posicao_coelho_Z)
                 * Matrix_Scale(ESCALA_COELHOS, ESCALA_COELHOS, ESCALA_COELHOS)
                 * Matrix_Rotate_Y((float)glfwGetTime() * 0.5f) ;
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(modelbunny));
    glUniform1i(object_id_uniform, BUNNY);
    DrawVirtualObject("bunny");
}


glm::vec4 colisao_esfera(GLFWwindow* window,glm::vec4 camera_position_c)
{

    camera_position_c  = posicao_inicial_camera; //retorna a camera pra posicao inicial




    return camera_position_c;

}

glm::vec4 colisao_coelho(GLFWwindow* window,glm::vec4 camera_position_c)
{


    printf("MENSAGEM DE COLIDIU COM O COELHO E GANHOU AQUI");

    camera_position_c  = posicao_inicial_camera; //retorna a camera pra posicao inicial


    return camera_position_c;

}



glm::vec4 colisao_parede(GLFWwindow* window,glm::vec4 camera_position_c)
{



    camera_position_c  = posicao_inicial_camera; //retorna a camera pra posicao inicial


    return camera_position_c;

}



//FUNCAO DA DIFICULDADE DO JOGO
float dificuldade(GLFWwindow* window, bool& pressRightCtrl, bool& pressRightShift, bool& pressRightAlt)
{
    int stateRIGHTCTRL = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    if (stateRIGHTCTRL == GLFW_PRESS)
    {
        pressRightCtrl = true;
        pressRightShift = false;
        pressRightAlt = false;

    }

    int stateRIGHTSHIFT = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
    if (stateRIGHTSHIFT == GLFW_PRESS)
    {
        pressRightShift = true;
        pressRightCtrl = false;
        pressRightAlt = false;

    }

    int stateRIGHTALT = glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    if (stateRIGHTALT == GLFW_PRESS)
    {
        pressRightShift = false;
        pressRightCtrl = false;
        pressRightAlt = true;

    }

    if(!pressRightCtrl && !pressRightShift)
    {
        return VESFERASPADRAO;
    }




    if (pressRightShift)
    {
        return VESFERASHARD;
        //printf("\nHARD");
    }

    if (pressRightCtrl)
    {
        return VESFERASEASY;
        //printf("\nEASY");
    }
    return VESFERASPADRAO;
}


//FUNCAO PRA MOSTRAR QUANDO GANHA OU PERDE
void TextRendering_ShowText(GLFWwindow* window, int resultado)
{

    static float old_seconds = (float)glfwGetTime();
    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);
    float seconds = (float)glfwGetTime();

    float ellapsed_seconds = seconds - old_seconds;


    if (resultado == 2){
        if( ellapsed_seconds > 1.0f && ellapsed_seconds < 5.0f)
        {
            TextRendering_PrintString(window, "Voce Venceu!", 0.2f-13*charwidth, -0.0f+2*lineheight/10, 2.0f);
            printf("\n\nEntra aqui MENSAGEM DE VITÓRIA");
        }
        else if (ellapsed_seconds > 5.0f){
            ellapsed_seconds = 0;
        }
        resultado = 0;
        return;
    }
    else if (resultado == 1){
        if( ellapsed_seconds > 1.0f && ellapsed_seconds < 5.0f)
        {
            TextRendering_PrintString(window, "Voce Perdeu!", 0.2f-13*charwidth, -0.0f+2*lineheight/10, 2.0f);
            printf("\n\nEntra aqui MENSAGEM DE VITÓRIA");
        }
        else if (ellapsed_seconds > 5.0f){
            ellapsed_seconds = 0;
        }
        resultado = 0;
        return;
    }
    else
        return;
    }

    void TextRendering_ShowTime(GLFWwindow* window, int resultado)
{



    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  buffer[20] = "?? seg";
    static int   numchars = 13;
    int i;

     float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;
    if (resultado == 2)
    {
       for(i=0;i<2000;i++)
       numchars = snprintf(buffer, 20, "Ganhou! %.2f seg ",ellapsed_seconds);

    }
    if (resultado == 1)
    {
        for(i=0;i<2000;i++)
        numchars = snprintf(buffer, 20, "Perdeu! %.2f seg ",ellapsed_seconds);
    }
    else
        numchars = snprintf(buffer, 20, "%.2f seg ",ellapsed_seconds);



    if (resultado == 1 || resultado == 2){


            old_seconds = seconds;
    }

    else
        TextRendering_PrintString(window, buffer, 0.5f-(numchars + 1)*charwidth, 0.5f-lineheight, 3.0f);
}



// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

