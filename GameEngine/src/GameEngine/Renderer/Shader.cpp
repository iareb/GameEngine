#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace GameEngine {

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		// Crea un nuovo oggetto shader di tipo vertex shader.
		// Restituisce un ID(GLuint) che rappresenta lo shader.
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Invia il codice sorgente del vertex shader a GL
		// Nota: std::string's .c_str termina con un NULL.
		const GLchar* source = vertexSource.c_str();
		
		//  vertexShader → ID dello shader.
		//	1 → numero di stringhe(una sola).
		//	&source → puntatore alla stringa del codice sorgente.
		//	0 → la stringa è terminata con NULL.
		glShaderSource(vertexShader, 1, &source, 0);

		// Compila il vertex shader.
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		// Controlla se la compilazione è andata a buon fine.
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			// Ottiene la lunghezza del log degli errori.
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// Recupera il log dell'errore dello shader e lo salva in un vector di GLchar.
			// maxLength include il carattere NULL.
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// Eliminiamo lo shader, perché la compilazione è fallita.
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Crea un fragment shader.
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Passa il codice sorgente del fragment shader.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// Se la compilazione fallisce, elimina entrambi gli shader per evitare memory leak.
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Entrambi gli shader sono stati compilati con successo.
		// Dobbiamo collegarli insieme in un program.
		// Un program è un oggetto OpenGL che contiene i vari shader.
		// Gli shader da soli non funzionano: devono essere collegati in un program.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Collega (attach) gli shader al program.
		// OpenGL ora sa che questo shader program utilizza questi due shader.
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Collega tutti gli shader in un unico shader program eseguibile.
		// OpenGL verifica compatibilità tra vertex e fragment shader(es.output del vertex shader = input del fragment shader).
		// Dopo il linking, gli shader non servono più direttamente, perché il programma ora è indipendente dagli shader originali.
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Eliminiamo il programma, non ci serve più.
			glDeleteProgram(program);
			// Cancelliamo anche gli shader.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Rimuove gli shader dal programma, ma senza eliminarli.
		// Questo perché dopo il linking, gli shader non servono più: il programma ha già tutto il necessario per funzionare.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

}
