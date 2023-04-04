#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include "glm/glm.hpp"

struct Node {
	glm::vec3 point;
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;
};

using PointIter = std::vector<glm::vec3>::iterator;

class KdTree
{
public:
	KdTree(std::vector<glm::vec3>& points);
	~KdTree();
	void Insert(const glm::vec3& point);
	std::vector<glm::vec3> Search(const glm::vec3& target, float r);
private:
	std::unique_ptr<Node> build(PointIter begin, PointIter end, int depth);
	void insert(std::unique_ptr<Node>& node, const glm::vec3& point, int depth);
	void search(const std::unique_ptr<Node>& node, const glm::vec3& target, double radius, int depth, std::vector<glm::vec3>& result);
private:
	std::unique_ptr<Node> _root;
};

KdTree::KdTree(std::vector<glm::vec3>& points)
{
	_root = build(points.begin(), points.end(), 0);
}

KdTree::~KdTree()
{
}

void KdTree::Insert(const glm::vec3& point) {
	this->insert(_root, point, 0);
}

std::vector<glm::vec3> KdTree::Search(const glm::vec3& target, float r) {
	std::vector<glm::vec3> result;
	search(_root, target, r, 0, result);
	return result;
}

void KdTree::insert(std::unique_ptr<Node>& node, const glm::vec3& point, int depth) {
	if (!node) {
		node = std::make_unique<Node>();
		node->point = point;
		return;
	}

	int axis = depth % 3;
	bool left = (axis == 0) ? point.x < node->point.x :
		(axis == 1) ? point.y < node->point.y :
		point.z < node->point.z;

	if (left) {
		insert(node->left, point, depth + 1);
		return;
	}
	insert(node->right, point, depth + 1);
}

void KdTree::search(const std::unique_ptr<Node>& node, const glm::vec3& target, double radius, int depth, std::vector<glm::vec3>& result) {
	if (!node) {
		return;
	}
	auto distance = glm::distance(target, node->point);
	if (distance <= radius) {
		result.push_back(node->point);
	}
	int axis = depth % 3;
	float diff = (axis == 0) ? target.x - node->point.x :
		(axis == 1) ? target.y - node->point.y :
		target.z - node->point.z;
	bool left = diff < 0;
	const auto& first = left ? node->left : node->right;
	const auto& second = left ? node->right : node->left;
	search(first, target, radius, depth + 1, result);

	if (diff * diff <= radius * radius) {
		search(second, target, radius, depth + 1, result);
	}
}

std::unique_ptr<Node> KdTree::build(PointIter begin, PointIter end, int depth) {
	if (begin == end) {
		return nullptr;
	}

	int axis = depth % 3;
	auto compare = [axis](const glm::vec3& a, const glm::vec3& b) {
		return (axis == 0) ? a.x < b.x : (axis == 1) ? a.y < b.y : a.z < b.z;
	};

	auto mid = begin + (end - begin) / 2;
	std::nth_element(begin, mid, end, [axis](const glm::vec3& a, const glm::vec3& b) {
		return (axis == 0) ? a.x < b.x : (axis == 1) ? a.y < b.y : a.z < b.z;
		});

	auto node = std::make_unique<Node>();
	node->point = *mid;
	node->left = build(begin, mid, depth + 1);
	node->right = build(mid + 1, end, depth + 1);

	return node;
}