#include "Layout_comparator.h"


bool Layout_bitmap_generator::is_figure_inside(GeometryItem* item)
{
	const Coord& left_top = bitmap_coords.at(0);
	const Coord& right_bot = bitmap_coords.at(1);
	bool flag = false;
	switch (item->type)
	{
	case it_boundary:
		for (size_t i = 0; i < item->coords.size() - 1; i++)
		{
			if (is_value_in_range(left_top.x, right_bot.x, item->coords[i].x) && is_value_in_range(right_bot.y, left_top.y, item->coords[i].y))
			{
				flag = true;
				break;
			}
		}
		break;
	case it_path:
	
		for (size_t i = 0; i < item->coords.size() - 1; i++)
		{
			GeometryItem_Path* temp = static_cast<GeometryItem_Path*>(item);
			//init 4 coords that transforms one fragment of the path into rectangle
			Coord rec_coords[4];
			if (temp->coords[i].x - temp->coords[i + 1].x == 0)// vertical orientation
			{
				Coord* p_top{ nullptr }, * p_bot{ nullptr };
				//finding top coord
				if (temp->coords[i].y > temp->coords[i + 1].y)
				{
					p_top = &temp->coords[i];
					p_bot = &temp->coords[i + 1];
				}
				else
				{
					p_bot = &temp->coords[i];
					p_top = &temp->coords[i + 1];
				}
				//calculating all angle coords
				rec_coords[0].x = rec_coords[3].x = p_top->x - temp->width;
				rec_coords[1].x = rec_coords[2].x = p_top->x + temp->width;
				
				rec_coords[0].y = rec_coords[1].y = p_top->y;
				rec_coords[2].y = rec_coords[3].y = p_bot->y;

			}
			else//horizontal orientation
			{
				Coord* p_left{ nullptr }, * p_right{ nullptr };
				//finding right coord
				if (temp->coords[i].x > temp->coords[i + 1].x)
				{
					p_left = &temp->coords[i + 1];
					p_right = &temp->coords[i];
				}
				else
				{
					p_left = &temp->coords[i];
					p_right = &temp->coords[i+1];
				}
				//calculating all_angle_coords
				rec_coords[0].x = rec_coords[3].x = p_left->x;
				rec_coords[1].x = rec_coords[2].x = p_right->x;
				rec_coords[0].y = rec_coords[1].y = p_left->y + temp->width;
				rec_coords[2].y = rec_coords[3].y = p_left->y - temp->width;

			}
			for (size_t j = 0; j < 4; j++)
			{
				if (is_value_in_range(left_top.x, right_bot.x, rec_coords[j].x) && is_value_in_range(right_bot.y, left_top.y, rec_coords[j].y))
				{
					flag = true;
					break;
				}
			}
			if (flag)
				break;
			

		}
		break;
	case it_box:
		for (size_t i = 0; i < item->coords.size() - 1; i++)
		{
			if (is_value_in_range(left_top.x, right_bot.x, item->coords[i].x) && is_value_in_range(right_bot.y, left_top.y, item->coords[i].y))
			{
				flag = true;
				break;
			}
		}
		break;
		//TODO: case structref for
	case it_structRef:
		break;
	default:
		break;
	}
	return flag;
}
void Layout_bitmap_generator::add_vect(std::vector <GeometryItem*>& dest, const std::vector<GeometryItem*>& source)
{
	for (size_t i = 0; i < source.size(); i++)
	{
		dest.push_back(source.at(i));
	}
}
bool Layout_bitmap_generator::is_value_in_range(const int32_t left, const int32_t right, const int32_t value)
{
	return(value < right && value > left) ? true : false;
}
int32_t Layout_bitmap_generator::calc_index_value(const int32_t from, const int32_t value, const uint32_t delta, const Variable variable,const Index index)
{
	int32_t temp{ 0 };

	switch (variable)
	{
	case Variable::x:
		temp = (value - from) / delta;
		break;
	case Variable::y:
		temp = (from - value) / delta;
		break;
	default:
		return -1;
	}
	switch (index)
	{
	case Index::first:
		return (temp > 0) ? temp : 0;
	case Index::last:
		return (temp > 0) ? temp : (fragments_size - 1);
	default:
		return -1;
	}
}
void Layout_bitmap_generator::alloc_fragment_matrix()
{
	if (fragments_size == 0)
		return;
	fragments = new(std::nothrow) Fragment * [fragments_size];
	for (size_t i = 0; i < fragments_size; i++)
	{
		fragments[i] = new Fragment[fragments_size];
	}
}
inline int32_t Layout_bitmap_generator::calculate_delta(const int32_t n1, const int32_t n2, const uint32_t split_count)
{
	return abs(n2 - n1) / split_count;
}
void Layout_bitmap_generator::init_fragments_coord(const uint32_t dx, const uint32_t dy)
{
	const Coord& left_top  = bitmap_coords.at(0);
	const Coord& right_bot = bitmap_coords.at(1);
	for (size_t i = 0; i < fragments_size; i++)
	{
		for (size_t j = 0; j < fragments_size; j++)
		{
			fragments[i][j].fragment_coordinates.push_back(Coord(left_top.x + i * dx, left_top.y - j * dy));//Init left_top fragment coord
			fragments[i][j].fragment_coordinates.push_back(Coord(left_top.x + (i + 1) * dx, left_top.y - (j + 1) * dy));//Init right_bot fragment coords
		}
	}
}
std::vector<GeometryItem*> Layout_bitmap_generator::get_all_items_from_layers()
{
	std::vector <GeometryItem*> all_items_from_layers;
	int32_t layer_counter = layers.size();
	for (size_t i = 0; i < data->libraries.size(); i++)
	{
		for (size_t j = 0; j < data->libraries[i]->layers.size(); j++)
		{
			for (size_t k = 0; k < layers.size(); k++)	
			{
				if (layers.at(k) == data->libraries[i]->layers[j].layer)
				{
					add_vect(all_items_from_layers, data->libraries[i]->layers[j].items);
					layer_counter--;
				}
			
			}
			if (layer_counter <= 0)
				goto END;
		}
	}
	END:
	return all_items_from_layers;
}
void Layout_bitmap_generator::push_GeometryItem_Box( GeometryItem * box)
{
	const Coord& left_top = bitmap_coords.at(0);
	const Coord& right_bot = bitmap_coords.at(1);
	const Coord& box_left_top = box->coords.at(0);
	const Coord& box_right_bot = box->coords.at(2);
	//case when the rectangle is out of bounds
	for (size_t i = 0; i < box->coords.size() - 1; i++)
	{
		//if(!is_value_in_range(lef))
	}
	int32_t min_j = calc_index_value(left_top.x, box_left_top.x, dx, Variable::x, Index::first);
	int32_t max_j = calc_index_value(left_top.x, box_left_top.x, dx, Variable::x, Index::last);
	int32_t min_i = calc_index_value(left_top.y, box_left_top.y, dx, Variable::y, Index::first);
	int32_t max_i = calc_index_value(left_top.y, box_right_bot.y, dx, Variable::y, Index::last);
	for (uint32_t i = min_i; i <= max_i; i++)
	{
		for (uint32_t j = min_j; j <= max_j; j++)
		{
			//fragments[i][j].items_in_fragment.push_back(box);
		}
	}


}
void Layout_bitmap_generator::push_GeometryItem_Path(const GeometryItem* path)
{



}
void Layout_bitmap_generator::push_GeometryItem_Boundary(const GeometryItem* boundary){}
void Layout_bitmap_generator::init_fragments_items()
{
	const std::vector <GeometryItem*> current_bitmap_items = get_all_items_from_layers();

	for (size_t i = 0; i < current_bitmap_items.size(); i++)
	{
		if (is_figure_inside(current_bitmap_items[i]))
		{
			switch (current_bitmap_items.at(i)->type)
			{
			case it_boundary:
				push_GeometryItem_Boundary(current_bitmap_items[i]);
				break;
			case it_path:
				push_GeometryItem_Path(current_bitmap_items[i]);
				break;
			case it_box:
				push_GeometryItem_Box(current_bitmap_items[i]);
				break;
				//case it_structRef:
					//break;
			default:
				break;
			}
		}
	}

}
bool Layout_bitmap_generator::init_generator(const LayoutData* data, const int32_t left_top_x, const int32_t left_top_y, const int32_t right_bot_x, const int32_t right_bot_y, const uint32_t fragment_size,const std::vector <int16_t> &layers)
{
	//Correct parameters checking
	if (!data || !fragment_size || layers.empty())
		return false;
	if (left_top_x > right_bot_x || left_top_y < right_bot_y)
		return false;

	this->data = const_cast<LayoutData*>(data);
	this->layers = layers;
	bitmap_coords.push_back(Coord(left_top_x, left_top_y));
	bitmap_coords.push_back(Coord(right_bot_x, right_bot_y));
	this->fragments_size = fragment_size;
	alloc_fragment_matrix();
	dx = calculate_delta(left_top_x, right_bot_x, fragments_size);
	dy = calculate_delta(right_bot_y, left_top_y, fragments_size);
	init_fragments_coord(dx,dy);
	init_fragments_items();


	return true;
}