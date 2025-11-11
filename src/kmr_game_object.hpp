#pragma once

#include "kmr_model.hpp"

// std
#include <memory>

namespace kmr
{
    struct Transform2dComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2()
        {
            const float c = cos(rotation);
            const float s = sin(rotation);
            glm::mat2 rotMat{{c, s}, {-s, c}};                  // glm uses column vectors, not row
            glm::mat2 scaleMat{{scale.x, 0.f}, {0.f, scale.y}}; // glm uses column vectors, not row
            return rotMat * scaleMat;
        }
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
        Transform2dComponent transform2d{};

    private:
        KmrGameObject(id_t id) : id{id} {}

        id_t id;
    };
} // namespace kmr