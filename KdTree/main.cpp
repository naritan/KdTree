#include <iostream>
#include "kdtree.h"

int main() {
    std::vector<glm::vec3> points = {
        { -25.3f, 84.1f, 43.2f },
        { 91.0f, -12.6f, 14.0f },
    };

    KdTree tree(points);
    tree.Insert({ 91.0f, -12.6f, 14.2f });
    tree.Insert({ -76.2f, 56.4f, -82.8f });
    tree.Insert({ 10.0f, -50.1f, 93.6f });
    tree.Insert({ 35.2f, 24.7f, -90.5f });
    tree.Insert({ -67.9f, 30.0f, 82.3f });
    tree.Insert({ 91.0f, -12.5f, 13.9f });
    tree.Insert({ 90.9f, -12.7f, 13.9f });

    glm::vec3 target(90.2f, -11.2f, 14.9f);
    auto result = tree.Search(target, 2.0f);

    std::cout << "x: " << target.x << " y: " << target.y << " z: " << target.z << std::endl;
    std::cout << "‚Ì‹ß‚­‚É‚ ‚é“_" << std::endl;
    for (auto r : result) {
        std::cout << "x: " << r.x << " y: " << r.y << " z: " << r.z << std::endl;
    }

}