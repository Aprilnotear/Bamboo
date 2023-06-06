#pragma once

#include "runtime/resource/asset/base/sub_mesh.h"

struct StaticVertex
{
	glm::vec3 position;
	glm::vec2 tex_coord;
	glm::vec3 normal;
};

struct SkeletalVertex : public StaticVertex
{
	glm::ivec4 bones;
	glm::vec4 weights;
};

namespace Bamboo
{
	class Mesh
	{
	public:
		std::vector<SubMesh> m_sub_meshes;
		std::vector<uint32_t> m_indices;

	private:
		friend class cereal::access;
		template<class Archive>
		void archive(Archive& ar) const
		{
			ar(m_sub_meshes);
			ar(m_indices.size());
		}

		template<class Archive>
		void save(Archive& ar) const
		{
			archive(ar);
		}

		template<class Archive>
		void load(Archive& ar)
		{
			archive(ar);
		}
	};
}