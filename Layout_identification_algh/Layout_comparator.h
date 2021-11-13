#pragma once
#include "LayoutData.hpp"


enum class Variable
{
	undefined = 0,
	x,
	y
};
enum class Index
{
	undefined = 0,
	first,
	last
};

struct Fragment
{
	std::vector <Coord>				fragment_coordinates;
	std::vector <GeometryItem*>		items_in_fragment;
	uint8_t**						bitmap;
	uint32_t						bitmap_size;

public:
	Fragment() :bitmap(nullptr){}
	~Fragment();
	//bool init_fragment(const std::vector<GeometryItem*> items);

	
};


class Layout_bitmap_generator {
private:
	LayoutData*				data;
	std::vector<Coord>		bitmap_coords;//bitmap_coords[0]-lefttop	bitmap_coords[1]-rightbot
	std::vector<int16_t>    layers;
	Fragment**				fragments;
	uint32_t				fragments_size;
	uint32_t				dx;
	uint32_t				dy;
	
public:
	Layout_bitmap_generator() :data(nullptr), fragments(nullptr), fragments_size(0), dx(0), dy(0) {}
	bool init_generator(const LayoutData* data,const int32_t left_top_x, const int32_t left_top_y, const int32_t right_bot_x, const int32_t right_bot_y, const uint32_t fragment_size, const std::vector <int16_t>& layers);
private:
	void alloc_fragment_matrix();
	inline int32_t calculate_delta(const int32_t n1, const int32_t n2, const uint32_t split_count);
	void init_fragments_coord(const uint32_t dx, const uint32_t dy);
	void init_fragments_items();
	std::vector<GeometryItem*> get_all_items_from_layers();
	void add_vect(std::vector <GeometryItem*>& dest, const std::vector<GeometryItem*>& source);
	bool is_value_in_range(const int32_t left, const int32_t right, const int32_t value);
	int32_t calc_index_value(const int32_t from, const int32_t value, const uint32_t delta,const Variable variable,const Index index);
	bool is_figure_inside(GeometryItem* item);
	//Pushing items into fragments
	void push_GeometryItem_Box(const GeometryItem* box);
	void push_GeometryItem_Path(const GeometryItem* path);
	void push_GeometryItem_Boundary(const GeometryItem* boundary);



};


class Layout_hash_generator {
private:
	Layout_bitmap_generator*	layout_bitmap;
	std::string					hash;
	bool						hash_correct;

public:


private:
};

class Layout_comparator {
private:
	std::pair <Layout_bitmap_generator*, Layout_bitmap_generator*> generators;


};