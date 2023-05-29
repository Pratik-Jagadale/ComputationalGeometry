#pragma once
#include "GLHeadersAndMacros.h"

class MarbleShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint projectMatrixUniform; // model View Projection
    GLuint viewMatrixUniform;    // model View Projection
    GLuint modelMatrixUniform;   // model View Projection

    GLuint lightPosUniform;
    GLuint scaleUniform;
    GLuint marbleColorUniform;
    GLuint veinColorUniform;

    GLuint textureSamplerUniform;

    float lightPos[4] = {0.0f, 0.0f, 4.0f, 1.0f};

    float marbleColor[3] = {0.7f, 0.7f, 0.7f}; // marble
    float veinColor[3] = {0.0f, 0.15f, 0.0f};  // marble

    GLfloat scaleFactor = 2.0f;

    // Function Declartions
    BOOL initialize_marbleShaderObject(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\marble.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\marble.fs", FRAGMENT);

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

        marbleColorUniform = glGetUniformLocation(shaderProgramObject, "MarbleColor");
        veinColorUniform = glGetUniformLocation(shaderProgramObject, "VeinColor");
        return TRUE;
    }

    void update_CloudShaderObjectUniforms(void)
    {
        glUniform3fv(lightPosUniform, 1, lightPos);
        glUniform1f(scaleUniform, scaleFactor);

        glUniform3fv(marbleColorUniform, 1, marbleColor);
        glUniform3fv(veinColorUniform, 1, veinColor);
    }

    void uninitialize_WoodShaderObject(void)
    {
    }
};
