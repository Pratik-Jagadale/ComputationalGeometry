#pragma once
#include "GLHeadersAndMacros.h"
#include "GLShaders.h"

class CloudShader
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
    GLuint skyColorUniform;
    GLuint cloudColorUniform;

    GLuint textureSamplerUniform;

    float lightPos[4] = {0.0f, 0.0f, 4.0f, 1.0f};

    float skyColor[3] = {0.0f, 0.0f, 0.2f};
    float cloudColor[3] = {0.6f, 0.6, 0.6f};

    GLfloat scaleFactor = 2.0f;
    BOOL isScaled = FALSE;

    BOOL initialize_cloudShaderObject(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\cloud.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\cloud.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, PRJ_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, PRJ_ATTRIBUTE_NORMAL, "a_normal");

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

        skyColorUniform = glGetUniformLocation(shaderProgramObject, "SkyColor");     // Sky related uniform
        cloudColorUniform = glGetUniformLocation(shaderProgramObject, "CloudColor"); // Sky related uniform
        return TRUE;
    }

    void update_CloudShaderObjectUniforms(void)
    {
        glUniform3fv(lightPosUniform, 1, lightPos);
        glUniform1f(scaleUniform, scaleFactor);

        glUniform3fv(skyColorUniform, 1, skyColor);
        glUniform3fv(cloudColorUniform, 1, cloudColor);

        if (isScaled == TRUE)
        {
            scaleFactor = scaleFactor - 0.004f;

            if (scaleFactor < 1.0f)
                isScaled = FALSE;
        }
        else
        {
            // scaleFactor = scaleFactor + 0.1f;
            scaleFactor = scaleFactor + 0.004f;
            if (scaleFactor > 1.5f)
                isScaled = TRUE;
        }
    }

    void uninitialize_WoodShaderObject(void)
    {
    }
};