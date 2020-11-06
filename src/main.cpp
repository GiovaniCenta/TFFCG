//#include "inicialize.cpp"
#include "loadFunctions.cpp"

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

//LIMITES DO MAPA


        #define LIMITEHORIZONTAL 5.0
        #define LIMITEVERTICAL 3.0
        #define LIMITEFRENTEATRAS 5.0
        #define SKYBOXTAM 5.0


#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"
void carregaObjetos(int argc, char* argv[]);
void desenhaObjetos();
void teste();


int main(int argc, char* argv[])
{


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


    //funcao que vai carregar os objetos que serão usados
    carregaObjetos(argc, argv);


    TextRendering_Init();
    glEnable(GL_DEPTH_TEST);
    glm::mat4 the_projection;
    glm::mat4 the_model;
    glm::mat4 the_view;


    //posicao inicial da camera
    glm::vec4 camera_position_c  = glm::vec4(0.0,0.0,2.5,1.0f);


    // Ficamos em loop, renderizando, até que o usuário feche a janela


    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_id);

        float r = g_CameraDistance;
        float y = r*sin(g_CameraPhi);
        float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
        float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);


        glm::vec4 camera_lookat_l    = glm::vec4(0.0f,0.0f,0.0f,1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
        glm::vec4 camera_view_vector = glm::vec4(-x, -y, -z, 0.0);
        glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0,0.0f,0.0f); // Vetor "up"

        //minha fpsCamera

        camera_position_c = fpsCamera(window,camera_position_c,camera_view_vector,camera_up_vector);



        glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

        // Agora computamos a matriz de Projeção.
        glm::mat4 projection;


        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -10.0f; // Posição do "far plane"

        if (g_UsePerspectiveProjection)
        {

            float field_of_view = 3.141592 / 3.0f;
            projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        }
        else
        {

            float t = 1.5f*g_CameraDistance/2.5f;
            float b = -t;
            float r = t*g_ScreenRatio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
        }




        glUniformMatrix4fv(view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));








        //teste();







        desenhaObjetos();

        //TextRendering_ShowEulerAngles(window);
        //TextRendering_ShowProjection(window);
        TextRendering_ShowFramesPerSecond(window);


        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}




int y = 0;


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



    //Mecanica de corrida, segurou shift anda 2 vezes mais rapido
    if (stateSHIFT == GLFW_PRESS)
    {
        speed = speed * 2;
    }





    if (stateW == GLFW_PRESS && camera_position_c.x < LIMITEFRENTEATRAS && camera_position_c.z < LIMITEFRENTEATRAS && camera_position_c.z > -LIMITEFRENTEATRAS && camera_position_c.x > -LIMITEFRENTEATRAS)
    {
        printf("\n%f", camera_position_c.x);
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
    if (stateSPACE == GLFW_PRESS)
    {


            camera_position_c.y = 0.1;



    }

//mecanica de agachamento, mesmo problema
     else if (stateCTRL == GLFW_PRESS)
    {


            camera_position_c.y = -0.1;

        }


    else{
        camera_position_c.y = 0.0;
    }

    return camera_position_c;
}



 void carregaObjetos(int argc, char* argv[]){


    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/grama.jpg");      // TextureImage0
    LoadTextureImage("../../data/tc-earth_nightmap_citylights.gif"); // TextureImage1
    LoadTextureImage("../../data/NIGHTSKY.jpg");

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ObjModel spheremodel("../../data/sphere.obj");
    ComputeNormals(&spheremodel);
    BuildTrianglesAndAddToVirtualScene(&spheremodel);

    ObjModel manmodel("../../data/a.obj");
    ComputeNormals(&manmodel);
    BuildTrianglesAndAddToVirtualScene(&manmodel);

    ObjModel bunnymodel("../../data/bunny.obj");
    ComputeNormals(&bunnymodel);
    BuildTrianglesAndAddToVirtualScene(&bunnymodel);

    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

    ObjModel spheremodel2("../../data/cow.obj");
    ComputeNormals(&spheremodel2);
    BuildTrianglesAndAddToVirtualScene(&spheremodel2);

    //ObjModel handgunmodel("../../data/sphere.obj");
    //ComputeNormals(&handgunmodel);
    //BuildTrianglesAndAddToVirtualScene(&handgunmodel);

    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

}


void desenhaObjetos(){

        #define SPHERE 0
        #define BUNNY  1
        #define CHAO  2
        #define HOMEM 3
        #define SPHERE2 4
        #define SKY_BOX 5
        #define CEU 6

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem
        // Desenhamos o modelo da esfera
        model = Matrix_Translate(0.0f,0.5f,0.0f) * Matrix_Scale(0.5f, 0.5f, 0.5f)
              * Matrix_Rotate_Z(0.6f)
              * Matrix_Rotate_X(0.2f)
              * Matrix_Rotate_Y(g_AngleY + (float)glfwGetTime() * 0.1f);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, SPHERE);
        DrawVirtualObject("sphere");

        // Desenhamos o modelo do coelho
        model = Matrix_Translate(1.0f,0.0f,1.0f)
        * Matrix_Scale(-0.5f, -0.5f, -0.5f)
        * Matrix_Rotate_X(3.14) ;
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, BUNNY);
        DrawVirtualObject("bunny");



        // VACA
        model = Matrix_Translate(2.0f,0.0f,4.0f);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, HOMEM);
        DrawVirtualObject("cow");






















        //LIMITES DO MAPA


        // Desenhamos o plano do chão
        model = Matrix_Translate(0.0f,-0.5f,0.0f);
        model = model * Matrix_Scale(SKYBOXTAM, 0.0f, SKYBOXTAM);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, CHAO);
        DrawVirtualObject("plane");



        //SKY BOX LATERAL ESQUERDA
        model = Matrix_Translate(-LIMITEHORIZONTAL,0.0f,0.0f) * Matrix_Scale(0.0f, SKYBOXTAM, SKYBOXTAM)
        *Matrix_Rotate_Z(M_PI_2) ;
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, SKY_BOX);
        DrawVirtualObject("plane");


        //SKY BOX LATERAL DIREITA
        model = Matrix_Translate(LIMITEHORIZONTAL,0.0f,0.0f) * Matrix_Scale(0.0f, SKYBOXTAM, SKYBOXTAM)
        *Matrix_Rotate_Z(M_PI_2);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, SKY_BOX);
        DrawVirtualObject("plane");

        //SKY BOX SUPERIOR
        model = Matrix_Translate(0.0f,LIMITEVERTICAL,0.0f) * Matrix_Scale(SKYBOXTAM, 0.0f, SKYBOXTAM);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, CEU);
        DrawVirtualObject("plane");

        //SKY BOX FRONTAL
        model = Matrix_Translate(0.0f,0.0f,LIMITEFRENTEATRAS) * Matrix_Scale(SKYBOXTAM, SKYBOXTAM,0.0f )
        *Matrix_Rotate_X(M_PI_2);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, SKY_BOX);
        DrawVirtualObject("plane");

        //SKY BOX FRONTAL
        model = Matrix_Translate(0.0f,0.0f,-LIMITEFRENTEATRAS) * Matrix_Scale(SKYBOXTAM, SKYBOXTAM,0.0f )
        *Matrix_Rotate_X(M_PI_2);
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
        glUniform1i(object_id_uniform, SKY_BOX);
        DrawVirtualObject("plane");


        }






// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

