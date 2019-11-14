// Based on code from ogl2brick-2.0 at
// http://developer.3dlabs.com/downloads/glslexamples/index.htm

#include <fcntl.h>
#if defined(OSX) || defined(Linux)
	#include <unistd.h>
#else
	#include <io.h>
#endif

int shaderSize(char *fileName)
{
    //
    // Returns the size in bytes of the shader fileName.
    // If an error occurred, it returns -1.
    //
    int fd;
    int count = -1;

    fd = open(fileName, O_RDONLY);
    if (fd != -1)
    {
        count = lseek(fd, 0, SEEK_END) + 1;
        close(fd);
    }

    return count;
}

int readShader(char *fileName,char *shaderText, int size)
{
    //
    // Reads a shader from the supplied file and returns the shader in the
    // arrays passed in. Returns 1 if successful, 0 if an error occurred.
    // The parameter size is an upper limit of the amount of bytes to read.
    // It is ok for it to be too big.
    //
    FILE *fh;
    int count;


    // Open the file
    fh = fopen(fileName, "r");
    if (!fh)
        return -1;

    // Get the shader from a file.
    fseek(fh, 0, SEEK_SET);
    count = (int) fread(shaderText, 1, size, fh);
    shaderText[count] = '\0';

    if (ferror(fh))
        count = 0;

    fclose(fh);
    return count;
}



int readShaderSource(char *fileName, GLchar **Shader)
{
    int shSize;

    //
    // Allocate memory to hold the source of our shaders.
    //
    shSize = shaderSize(fileName);

    if (shSize == -1)
    {
        printf("Cannot determine size of the shader %s\n", fileName);
        return 0;
    }

    *Shader = (GLchar *) malloc(shSize);

    // Read the source code
    if (!readShader(fileName,*Shader, shSize))
    {
        printf("Cannot read the file %s\n", fileName);
        return 0;
    }

    return 1;
}

// Courtesy of Andy Eyster
void printOGLShaderLog(GLuint shader)
{
	GLint log_length;
	char* log;

	// Get error log length
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&log_length);

	// Dynamically allocate log buffer
	log = new char[log_length];
	
	// Get error log
	glGetShaderInfoLog(shader,log_length,&log_length,log);

	printf("%s\n",log);
	delete log;
}

// Courtesy of Andy Eyster
void printOGLProgramLog(GLuint program)
{
	GLint log_length;
	char* log;

	// Get error log length
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&log_length);

	// Dynamically allocate log buffer
	log = new char[log_length];
	
	// Get error log
	glGetProgramInfoLog(program,log_length,&log_length,log);

	printf("%s\n",log);
	delete log;
}

// Routine to load and compile shaders
GLuint load_shaders(GLchar* vertexFile, GLchar* fragmentFile)
{
	// Shader source strings
	GLchar* vertexSrc;
	GLchar* fragmentSrc;

	// GLSL flags
	GLint compiled, linked;

	// Read shader source from files
	readShaderSource(vertexFile,&vertexSrc);
	readShaderSource(fragmentFile,&fragmentSrc);

#ifndef OSX
	// Check for shader capabilities
	if (!glewIsSupported("GL_ARB_vertex_shader"))
	{
		printf("Programmable vertex shaders are not supported.\n");
		exit(0);
	}
	else if (!glewIsSupported("GL_ARB_fragment_shader"))
	{
		printf("Programmable fragment shaders are not supported.\n");
		exit(0);
	}
#endif
	// Create shader objects
	GLuint vertexObj = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragObj = glCreateShader(GL_FRAGMENT_SHADER);

	// Associate source with shader objects
	glShaderSource(vertexObj,1,(const GLchar**) &vertexSrc,NULL);
	glShaderSource(fragObj,1,(const GLchar**) &fragmentSrc,NULL);

	// Compile vertex shader
	glCompileShader(vertexObj);
	glGetShaderiv(vertexObj,GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Error compiling vertex shader.\n");
		printOGLShaderLog(vertexObj);
		exit(0);
	}

	// Compile fragment shader
	glCompileShader(fragObj);
	glGetShaderiv(fragObj,GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Error compiling fragment shader.\n");
		printOGLShaderLog(fragObj);
		exit(0);
	}

	// Create shader program object
	GLuint shaderProg = glCreateProgram();

	// Attach shader objects to program
	glAttachShader(shaderProg, vertexObj);
	glAttachShader(shaderProg, fragObj);

	// Link shaders
	glLinkProgram(shaderProg);
	glGetProgramiv(shaderProg,GL_LINK_STATUS, &linked);
	if (!linked)
	{
		printf("Shader program failed to link.\n");
		printOGLProgramLog(shaderProg);
		exit(0);
	}

	return shaderProg;
}
