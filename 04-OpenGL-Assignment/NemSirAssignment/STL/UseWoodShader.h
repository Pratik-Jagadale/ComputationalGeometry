#pragma once
#include "GLHeadersAndMacros.h"
#include "GLLog.h"
#include "GLShaders.h"

class WoodShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint projectMatrixUniform; // model View Projection
    GLuint viewMatrixUniform;    // model View Projection
    GLuint modelMatrixUniform;   // model View Projection

    GLuint textureSamplerUniform;

    GLuint lightPosUniform;
    GLuint scaleUniform;

    GLuint LightWoodColor;
    GLuint DarkWoodColor;
    GLuint RingFreq;
    GLuint LightGrains;
    GLuint DarkGrains;
    GLuint GrainThreshold;
    GLuint NoiseScale;
    GLuint Noisiness;
    GLuint GrainScale;

    float lightPos[4] = {0.0f, 0.0f, 4.0f, 1.0f};

    float LightWoodColor_Factor[3] = {0.6f, 0.3f, 0.1f};
    float DarkWoodColor_Factor[3] = {0.4f, 0.2f, 0.07f};
    float RingFreq_Factor = 4.0f;
    float LightGrains_Factor = 1.0f;
    float DarkGrains_Factor = 0.0f;
    float GrainThreshold_Factor = 0.5f;
    float NoiseScale_Factor[3] = {0.5f, 0.1f, 0.1f};
    float Noisiness_Factor = 3.0f;
    float GrainScale_Factor = 27.0f;
    GLfloat scaleFactor = 2.0f;

    /* Function Definitions */
    BOOL initialize_WoodShaderObject(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\wood.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\wood.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, PVG_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, PVG_ATTRIBUTE_NORMAL, "a_normal");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;
        // post link - getting
        projectMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        lightPosUniform = glGetUniformLocation(shaderProgramObject, "LightPos");
        scaleUniform = glGetUniformLocation(shaderProgramObject, "Scale");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

        LightWoodColor = glGetUniformLocation(shaderProgramObject, "LightWoodColor");
        DarkWoodColor = glGetUniformLocation(shaderProgramObject, "DarkWoodColor");
        RingFreq = glGetUniformLocation(shaderProgramObject, "RingFreq");
        LightGrains = glGetUniformLocation(shaderProgramObject, "LightGrains");
        DarkGrains = glGetUniformLocation(shaderProgramObject, "DarkGrains");
        GrainThreshold = glGetUniformLocation(shaderProgramObject, "GrainThreshold");
        NoiseScale = glGetUniformLocation(shaderProgramObject, "NoiseScale");
        Noisiness = glGetUniformLocation(shaderProgramObject, "Noisiness");
        GrainScale = glGetUniformLocation(shaderProgramObject, "GrainScale");

        return TRUE;
    }

    void update_WoodShaderObjectUniforms(void)
    {
        glUniform3fv(lightPosUniform, 1, lightPos);
        glUniform1f(scaleUniform, scaleFactor);

        glUniform3fv(LightWoodColor, 1, LightWoodColor_Factor);
        glUniform3fv(DarkWoodColor, 1, DarkWoodColor_Factor);
        glUniform3fv(NoiseScale, 1, NoiseScale_Factor);

        glUniform1f(RingFreq, RingFreq_Factor);
        glUniform1f(LightGrains, LightGrains_Factor);
        glUniform1f(DarkGrains, DarkGrains_Factor);

        glUniform1f(GrainThreshold, GrainThreshold_Factor);
        glUniform1f(Noisiness, Noisiness_Factor);
        glUniform1f(GrainScale, GrainScale_Factor);
    }

    void uninitialize_WoodShaderObject(void)
    {
    }
};
