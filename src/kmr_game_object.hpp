#pragma once

#include "kmr_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace kmr
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{}; // in radians

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();
        glm::mat3 normalMat();
    };

    class KmrGameObject
    {
    public:
        using id_t = unsigned int;

        static KmrGameObject createGameObject()
        {
            static id_t currentId = 0;
            return KmrGameObject{currentId++};
        }

        KmrGameObject(const KmrGameObject &) = delete;
        KmrGameObject &operator=(const KmrGameObject &) = delete;
        KmrGameObject(KmrGameObject &&) = default;
        KmrGameObject &operator=(KmrGameObject &&) = default;

        id_t getId() const { return id; }

        std::shared_ptr<KmrModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        KmrGameObject(id_t id) : id{id} {}

        id_t id;
    };
} // namespace kmr