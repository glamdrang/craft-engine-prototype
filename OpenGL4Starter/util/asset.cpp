#include "asset.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

class AssImpAsset : public IAsset
{
private:
	std::string _filename;

	// Assimp scene and general memory buffers loaded
	bool _assimpLoaded;
	// Our structure is loaded
	bool _loaded;
	// OpenGL memory buffers loaded.
	bool _glLoaded;

	GLuint _glid_bufferArray;
	GLuint _glid_vertexBuffer;
	GLuint _glid_indexBuffer;
	std::vector<GLuint> _glidVec_textures;

	size_t _vertexSize;
	size_t _vertexBufferSize;
	size_t _indexBufferSize;

	struct _Vertex
	{
		aiVector3D pos;
		aiVector2D tex;
		aiVector3D norm;
	};

	struct _Mesh
	{
		size_t start_vertex;
		size_t length_vertex;

		size_t start_index;
		size_t length_index;

		size_t start_element;
		size_t length_element;

		unsigned int material;
	};

	struct _Material
	{
		unsigned int texture_diffuse;
	};

	struct _Texture
	{
		unsigned int surface;
	};

	std::vector<_Mesh> _vec_meshes;
	std::vector<_Texture> _vec_textures;
	std::vector<_Material> _vec_materials;

	std::vector<_Vertex> _vecBuf_verticies;
	std::vector<unsigned int> _vecBuf_indicies;
	std::vector<SDL_Surface*> _vecBuf_surfaces;

public:
	AssImpAsset(std::string filename)
		: _filename(filename)
	{
		_assimpLoaded = false;
		_glLoaded = false;
		_loaded = false;
	}

	virtual void load()
	{
		_vertexBufferSize = 0;
		_indexBufferSize = 0;

		size_t directory_last_split;
		{
			size_t sp_bs = _filename.rfind('\\');
			size_t sp_fs = _filename.rfind('/');
			if		(sp_fs == std::string::npos)	directory_last_split = sp_bs;
			else if (sp_bs == std::string::npos)	directory_last_split = sp_fs;
			else									directory_last_split = std::max(sp_bs, sp_fs);
		}
		const std::string directory = _filename.substr(0, directory_last_split + 1);

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_filename,
			aiProcess_GenSmoothNormals |
			aiProcess_Triangulate |				// force triangles
			aiProcess_SortByPType
			);

		_vec_meshes.resize(scene->mNumMeshes);
		for (unsigned int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++)
		{
			const aiMesh& ai_mesh = *scene->mMeshes[iMesh];
			_Mesh mesh;

			mesh.start_vertex = _vertexBufferSize;
			for (unsigned int iVertex = 0; iVertex < ai_mesh.mNumVertices; iVertex++)
			{
				_Vertex v;

				v.pos = ai_mesh.mVertices[iVertex];
				const aiVector3D* tex = ai_mesh.HasTextureCoords(0) ? &ai_mesh.mTextureCoords[0][iVertex] : &Zero3D;
				v.tex.x = tex->x;
				v.tex.y = tex->y;
				v.norm = ai_mesh.HasNormals() ? ai_mesh.mNormals[iVertex] : Zero3D;

				_vecBuf_verticies.push_back(v);
				_vertexBufferSize += sizeof(_Vertex);
			}
			mesh.length_vertex = _vertexBufferSize - mesh.start_vertex;

			mesh.start_index = _indexBufferSize;
			const unsigned int startIndex = (unsigned int)mesh.start_index;
			for (unsigned int iFace = 0; iFace < ai_mesh.mNumFaces; iFace++)
			{
				const aiFace& ai_face = ai_mesh.mFaces[iFace];

				_vecBuf_indicies.push_back(startIndex + ai_face.mIndices[0]);
				_vecBuf_indicies.push_back(startIndex + ai_face.mIndices[1]);
				_vecBuf_indicies.push_back(startIndex + ai_face.mIndices[2]);
				_indexBufferSize += sizeof(unsigned int) * 3;
			}
			mesh.length_index = _indexBufferSize - mesh.start_index;

			mesh.start_element = mesh.start_index / (sizeof(unsigned int));
			mesh.length_element = mesh.length_index / (sizeof(unsigned int));

			mesh.material = ai_mesh.mMaterialIndex;
			_vec_meshes[iMesh] = mesh;
		}

		_vec_materials.resize(scene->mNumMaterials);
		for (unsigned int iMaterial = 0; iMaterial < scene->mNumMaterials; iMaterial++)
		{
			_Material mat;
			const aiMaterial* ai_material = scene->mMaterials[iMaterial];

			aiString texturePath;

			const unsigned int numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);

			if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0 && ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
			{
				_Texture tex;

				// Load file
				std::string tex_filename = directory + texturePath.data;
				SDL_Surface *image;
				image = IMG_Load(tex_filename.c_str());
				if (!image) {
					std::cerr << "Failed to open image \"" << tex_filename << "\" for an asset material." << std::endl;
					continue;
				}
				_vecBuf_surfaces.push_back(image);
				tex.surface = _vecBuf_surfaces.size() - 1;

				_vec_textures.push_back(tex);
			}
			mat.texture_diffuse = _vec_textures.size() - 1;
			_vec_materials.push_back(mat);
		}

		_assimpLoaded = true;
		_loaded = true;
	}

	virtual void unload()
	{
		_vecBuf_verticies.resize(0);
		_vecBuf_indicies.resize(0);

		for (int i = 0; i < _vecBuf_surfaces.size(); i++) { SDL_FreeSurface(_vecBuf_surfaces[i]); }
		_vecBuf_surfaces.resize(0);

		_assimpLoaded = false;
	}

	virtual void gl_load()
	{
		// Gen and bind BufferArray (VBO)
		// This is all the meshes.
		glGenVertexArrays(1, &_glid_bufferArray);
		glBindVertexArray(_glid_bufferArray);

		glGenBuffers(2, &_glid_vertexBuffer); // overflows into  &_glid_indexBuffer
		glBindBuffer(GL_ARRAY_BUFFER, _glid_vertexBuffer); // this is bound inside the VBO
		glBufferData(GL_ARRAY_BUFFER, _vertexBufferSize, _vecBuf_verticies.data(), GL_STATIC_DRAW);

		// Positions
		glVertexAttribPointer(
			0,                  // attribute 0. Must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(_Vertex),    // stride
			(void*)0            // array buffer offset
			);
		glEnableVertexAttribArray(0);

		// Texture Coordinates
		glVertexAttribPointer(
			1,                  // attribute 1. Must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(_Vertex),    // stride
			(void*)(sizeof(aiVector3D))   // array buffer offset
			);
		glEnableVertexAttribArray(1);

		// Normals
		glVertexAttribPointer(
			2,                  // attribute 2. Must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(_Vertex),    // stride
			(void*)(sizeof(aiVector3D) + sizeof(aiVector2D))   // array buffer offset
			);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glid_indexBuffer); // this is bound inside the VBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexBufferSize, _vecBuf_indicies.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		// Now we stuff all the textures into memory.
		size_t textures = _vecBuf_surfaces.size();
		_glidVec_textures.resize(textures);
		glGenTextures(textures, _glidVec_textures.data());

		for (int i = 0; i < textures; i++)
		{
			SDL_Surface* surface = _vecBuf_surfaces[i];
			GLuint texture = _glidVec_textures[i];

			glBindTexture(GL_TEXTURE_2D, texture);

			int mode = GL_RGB;

			if (surface->format->BytesPerPixel == 4) {
				mode = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		_glLoaded = true;
	}

	virtual void gl_render()
	{
		glBindVertexArray(_glid_bufferArray);
		for (int i = 0; i < _vec_meshes.size(); i++)
		{
			_Mesh& mesh = _vec_meshes[i];
			glActiveTexture(GL_TEXTURE0 + 1); glBindTexture(GL_TEXTURE_2D, _glidVec_textures[_vec_materials[mesh.material].texture_diffuse]);
			glActiveTexture(GL_TEXTURE0);
			glDrawElements(GL_TRIANGLES, mesh.length_element, GL_UNSIGNED_INT, (void*)mesh.start_index);
		}
		glBindVertexArray(0);
	}

	virtual void gl_unload()
	{
		glDeleteBuffers(2, &_glid_vertexBuffer);
		glDeleteVertexArrays(1, &_glid_bufferArray);
		_glLoaded = false;
	}

	virtual void destroy()
	{
		if (_glLoaded)
			this->gl_unload();
		if (_assimpLoaded)
			this->unload();
		delete this;
	}

	virtual std::string getName()
	{
		return _filename;
	}
};

IAsset* assetFromFile(std::string filename)
{
	return new AssImpAsset(filename);
}