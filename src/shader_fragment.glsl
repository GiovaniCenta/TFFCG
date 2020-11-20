#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE 0
#define BUNNY  1
#define CHAO  2
#define HOMEM  3
#define SPHERE2 4
#define SKY_BOX 5
#define CEU 6
#define CUBE 7
#define SKY_BOX_0 8
#define SKY_BOX_1 9
#define SKY_BOX_2 10
#define SKY_BOX_3 11
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;


// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec3 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{

    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    //lanterna
    vec4 lanterna = camera_position;
    vec4 dirLanterna = camera_position - position_world;



    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(dirLanterna);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2*n*dot(n,l); // PREENCHA AQUI o vetor de reflexão especular ideal

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    if ( object_id == SPHERE  )
    {
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

        vec4 pvector = position_model-bbox_center;

        float px = pvector.x;
        float py = pvector.y;
        float pz = pvector.z;

        float ro = length(pvector);//ro - raio da esfera : distancia do centro até o fim do vetor
        float theta = atan(px, pz); //angulo entre os vetores na direcao x e z
        float phi = asin(py/ro);//angulo entre os vetores na direcao y e ro

        U = (theta+M_PI)/(2*M_PI); //U = theta normalizado
        V = (phi+ M_PI_2)/M_PI;  //V = phi normalizado
    }
    else if ( object_id == BUNNY )
    {
        Kd = vec3(1,1,1);
        Ks = vec3(1,1,1);
        Ka = vec3(1,1,1);
        q = 32.0;

        /*
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

       U = (position_model.x - minx)/(maxx-minx);
        V = (position_model.y - miny)/(maxy-miny);
        */
    }





    else if(object_id == CHAO || object_id == CEU || object_id == SKY_BOX_0 || object_id == SKY_BOX_1 || object_id == SKY_BOX_2 || object_id == SKY_BOX_3 )
    {

        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
       U = texcoords.x;
        V = texcoords.y;


    }
    /*else if ( object_id == CEU )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
       U = texcoords.x;
        V = texcoords.y;
    }
*/
    else if ( object_id == CUBE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
       U = texcoords.x;
        V = texcoords.y;
    }


    else if( object_id == HOMEM)
    {


        Kd = vec3(0.7,0.2,0.7);
        Ks = vec3(0.5,0.5,0.5);
        Ka = vec3(0.5,0.5,0.5);
        q = 32.0;

        /*
        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

       U = (position_model.x - minx)/(maxx-minx);
        V = (position_model.y - miny)/(maxy-miny);
        */
    }


    else // Objeto desconhecido = preto
    {
      vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2*n*dot(n,l); // PREENCHA AQUI o vetor de reflexão especular ideal

    // Parâmetros que definem as propriedades espectrais da superfície
    vec3 Kd; // Refletância difusa
    vec3 Ks; // Refletância especular
    vec3 Ka; // Refletância ambiente
    float q; // Expoente especular para o modelo de iluminação de Phong


        // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd*I*max(0,dot(n,l)); // PREENCHA AQUI o termo difuso de Lambert

    // Termo ambiente
    vec3 ambient_term = Ia*Ka; // PREENCHA AQUI o termo ambiente

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks*pow(max(0,dot(r,v)),q); // PREENCH AQUI o termo especular de Phong

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
    color = lambert_diffuse_term + ambient_term + phong_specular_term;
    }



    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 texturaChao = texture(TextureImage0, vec2(U,V)).rgb; // textura do chão

    vec3 texturaEsfera = texture(TextureImage1, vec2(U,V)).rgb; // bola



    vec3 texturaTeto = texture(TextureImage2,vec2(U,V)).rgb; //textura pro céu

    vec3 texturaParede = texture(TextureImage3, vec2(U,V)).rgb; // textura da parede

    vec3 texturaCoelho = texture(TextureImage4, vec2(U,V)).rgb;

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));




        // Espectro da fonte de iluminação
    vec3 I = vec3(1.0,1.0,1.0); // PREENCH AQUI o espectro da fonte de luz

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2,0.2,0.2); // PREENCHA AQUI o espectro da luz ambiente

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd*I*max(0,dot(n,l)); // PREENCHA AQUI o termo difuso de Lambert

    // Termo ambiente
    vec3 ambient_term = Ia*Ka; // PREENCHA AQUI o termo ambiente

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks*pow(max(0,dot(r,v)),q); // PREENCH AQUI o termo especular de Phong





   if (object_id ==  CHAO || object_id == SKY_BOX)
    color = texturaChao * (lambert + 1);
   else if (object_id ==  SPHERE)
    color = texturaEsfera * (lambert + 1);

    else if(object_id == SKY_BOX_0 || object_id == SKY_BOX_1 || object_id == SKY_BOX_2 || object_id == SKY_BOX_3 )
    color = texturaParede * (lambert + 1);
    else if (object_id ==  CEU)
    color = texturaTeto * (lambert + 1);

    else if (object_id == BUNNY)
    color = (lambert_diffuse_term + ambient_term + phong_specular_term);



    else
    color = (lambert_diffuse_term + ambient_term + phong_specular_term);


    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
}

