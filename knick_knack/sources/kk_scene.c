#include "kk_scene.h"

/*** Animazione occhi ***/
static const vect3f frame_scala_occhi[] = {
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.01},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.01},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.01},
	{1.0, 1.0, 0.33},
	{1.0, 1.0, 0.67},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00},
	{1.0, 1.0, 1.00}
};

/*** Animazione bolla ***/

/* Coordinata Y (costante) della bollicina */
#define BOLLA_Y 0.5

static const vect3f frame_pos_bolla[] = {
	{0.0, 0.0, 0.0},
	{-0.6, BOLLA_Y, 3.05},
	{-0.6 - 0.1530733, BOLLA_Y, 3.10},
	{-0.6 - 0.2828426, BOLLA_Y, 3.15},
	{-0.6 - 0.3695517, BOLLA_Y, 3.20},
	{-0.6 - 0.4, BOLLA_Y, 3.25},
	{-0.6 - 0.3695517, BOLLA_Y, 3.30},
	{-0.6 - 0.2828426, BOLLA_Y, 3.35},
	{-0.6 - 0.1530733, BOLLA_Y, 3.40},
	{-0.6, BOLLA_Y, 3.45},
	{-0.6 - 0.1530733, BOLLA_Y, 3.50},
	{-0.6 - 0.2828426, BOLLA_Y, 3.55},
	{-0.6 - 0.3695517, BOLLA_Y, 3.60},
	{-0.6 - 0.4, BOLLA_Y, 3.65},
	{-0.6 - 0.3695517, BOLLA_Y, 3.70},
	{-0.6 - 0.2828426, BOLLA_Y, 3.75},
	{-0.6 - 0.1530733, BOLLA_Y, 3.80},
	{-0.6, BOLLA_Y, 3.85},
	{-0.6 - 0.1530733, BOLLA_Y, 3.90},
	{-0.6 - 0.2828426, BOLLA_Y, 3.95},
	{-0.6 - 0.3695517, BOLLA_Y, 4.00},
	{-0.6 - 0.4, BOLLA_Y, 4.05},
	{-0.6 - 0.3695517, BOLLA_Y, 4.10},
	{-0.6 - 0.2828426, BOLLA_Y, 4.15},
	{-0.6 - 0.1530733, BOLLA_Y, 4.20},
	{-0.6, BOLLA_Y, 4.25},
	{-0.6 - 0.1530733, BOLLA_Y, 4.30},
	{-0.6 - 0.2828426, BOLLA_Y, 4.35},
	{-0.6 - 0.3695517, BOLLA_Y, 4.40},
	{-0.6 - 0.4, BOLLA_Y, 4.45},
	{-0.6 - 0.3695517, BOLLA_Y, 4.50},
	{-0.6 - 0.2828426, BOLLA_Y, 4.55},
	{-0.6 - 0.1530733, BOLLA_Y, 4.60},
	{-0.6, BOLLA_Y, 4.65},
	{-0.6 - 0.1530733, BOLLA_Y, 4.70},
	{-0.6 - 0.2828426, BOLLA_Y, 4.75},
	{-0.6 - 0.3695517, BOLLA_Y, 4.80},
	{-0.6 - 0.4, BOLLA_Y, 4.85},
	{-0.6 - 0.3695517, BOLLA_Y, 4.90},
	{-0.6 - 0.2828426, BOLLA_Y, 4.95},
	{-0.6 - 0.1530733, BOLLA_Y, 5.00},
	{-0.6, BOLLA_Y, 5.05},
};

ITEM sfondo_attr, corpo_attr, occhi_attr, bocca_attr, bottoni_attr,
	sopracciglia_attr, naso_attr, cravatta_attr, cappello_attr, tavolo_attr,
	cupola_attr, cupola_fronte_attr, cupola_retro_attr, igloo_attr, 
	lampada_attr, bollicine_attr;

void build_environment(void)
{
	/*** Impostazione ambiente ***/
	set_background(1.0, 1.0, 1.0, TRUE);
	set_ambient_light(1, 1, 1, 0.7);
	create_point_light(2.320, 10.203, 15.992, 1.0, 1.0, 1.0, 0.75, 35, "Luce_Lampada");
/*	create_warn_light(2.320, 10.203, 15.992, -0.083, -0.604, -0.792, 1.0, 1.0, 1.0, 4.75, 
		40, 5, 50.0, 1, 0, 0, 1, 5, "Luce_Lampada");*/
	create_point_light(2.996, 3.896, 5.777, 1.0, 1.0, 1.0, 0.75, 10, "Luce_Cupola");
/*	create_point_light(8.25, 9.5, 14.8, 1.0, 1.0, 1.0, 0.75, 55, "Light2");*/
/*	create_point_light(1.7, 3.55, 6.21, 1.0, 1.0, 1.0, 0.75, 10, "Light3");*/

	/*** Sfondo ***/
	sfondo_attr = create_attribute("sfondo_attr");
	set_color(sfondo_attr, 1.0, 1.0, 1.0, 1.0, 1.0);
	set_reflectivity(sfondo_attr, 0.0, 128);
	set_transparency(sfondo_attr, 0.4, 0.2, 5, 1.0);

	corpo_attr = create_attribute("corpo_attr");
	set_color(corpo_attr, 0.9, 0.9, 0.9, 0.6, 1.0);
	set_reflectivity(corpo_attr, 0.0, 128);

	occhi_attr = create_attribute("occhi_attr");
	set_color(occhi_attr, 0.03, 0.03, 0.03, 0.2, 0.8);
	set_reflectivity(occhi_attr, 0.125, 15);

	bocca_attr = create_attribute("bocca_attr");
	set_color(bocca_attr, 0.03, 0.03, 0.03, 0.2, 0.8);
	set_reflectivity(bocca_attr, 0.125, 15);

	bottoni_attr = create_attribute("bottoni_attr");
	set_color(bottoni_attr, 0.03, 0.03, 0.03, 0.2, 0.8);
	set_reflectivity(bottoni_attr, 0.125, 15);

	sopracciglia_attr = create_attribute("sopracciglia_attr");
	set_color(sopracciglia_attr, 0.03, 0.03, 0.03, 0.2, 0.8);
	set_reflectivity(sopracciglia_attr, 0.125, 15);

	naso_attr = create_attribute("naso_attr");
	set_color(naso_attr, 1.0, 0.44, 0.44, 0.6, 0.8);
	set_reflectivity(naso_attr, 0.0625, 256);

	cravatta_attr = create_attribute("cravatta_attr");
	set_color(cravatta_attr, 0.03, 1.00, 0.03, 0.2, 0.8);
	set_reflectivity(cravatta_attr, 0.125, 15);

	cappello_attr = create_attribute("cappello_attr");
	set_color(cappello_attr, 0.03, 0.03, 0.03, 0.2, 0.8);
	set_reflectivity(cappello_attr, 0.0625, 15);

	cupola_attr = create_attribute("cupola_attr");
	set_color(cupola_attr, 1.0, 1.0, 1.0, 0.95, 0.95);
	set_reflectivity(cupola_attr, 0.0, 12);

	cupola_fronte_attr = create_attribute("cupola_fronte_attr");
	set_color(cupola_fronte_attr, 0.68, 0.84, 1.0, 0.25, 0.25);
	set_reflectivity(cupola_fronte_attr, 0.24, 512);
	set_transparency(cupola_fronte_attr, 0.87, 0.68, 1, 1.0);

	cupola_retro_attr = create_attribute("cupola_retro_attr");
	set_color(cupola_retro_attr, 1.0, 1.0, 1.0, 1.0, 1.0);
	set_reflectivity(cupola_retro_attr, 0.14, 512);

	igloo_attr = create_attribute("igloo_attr");
	set_color(igloo_attr, 1.0, 1.0, 1.0, 1.0, 1.0);

	tavolo_attr = create_attribute("tavolo_attr");
	set_color(tavolo_attr, 0.0, 0.22265625, 0.1015625, 0.6, 1.0);
	set_reflectivity(tavolo_attr, 0.0, 128);

	lampada_attr = create_attribute("lampada_attr");
	set_color(lampada_attr, 1.0, 1.0, 1.0, 0.0, 1.0);
	set_reflectivity(lampada_attr, 0.45, 128);
	set_transparency(lampada_attr, 0.0625, 0.0, 1, 1.0);

	bollicine_attr = create_attribute("bollicine_attr");
	set_color(bollicine_attr, 0.85, 0.85, 1.0, 1.0, 1.0);
	set_transparency(bollicine_attr, 1.0, 0.0, 1, 1.0);
}

ITEM build_scene_frame(unsigned frameno, const char *scene_name)
{
ITEM scene, item;

	/*** Scena ***/
	item = build_static_scene();
	scene = create_list(item, (char*)scene_name);

	/*** Knick Knack ***/
	item = build_knick_knack(frameno);
	set_translate(item, 0.0, 0.0, M);
	scene = insert_in_list(scene, item);

	/*** Cupola ***/
	item = build_cupola(frameno);
	set_translate(item, 0.0, 0.0, 0.175); 
	set_scale(item, 7.0, 7.0, 7.0);
	scene = insert_in_list(scene, item);

	/*** Igloo ***/
	item = build_igloo(frameno);
	set_scale(item, 2.0, 2.0, 2.0);
	set_translate(item, -1.0, -2.0, M);
	scene = insert_in_list(scene, item);

	/*** Bolla 1 ***/
	if(frameno > 0) {
		item = read_nurbs("bolla.db");
		set_attribute(item, bollicine_attr);
		set_translate(item, frame_pos_bolla[frameno][0], frame_pos_bolla[frameno][1], frame_pos_bolla[frameno][2]);
		scene = insert_in_list(scene, item);
	}

	/*** Bolla 2 ***/
	if(frameno >= 10) {
		item = read_nurbs("bolla.db");
		set_attribute(item, bollicine_attr);
		set_translate(item, frame_pos_bolla[frameno - 10][0], frame_pos_bolla[frameno - 10][1], frame_pos_bolla[frameno - 10][2]);
		scene = insert_in_list(scene, item);
	}

	/*** Bolla 3 ***/
	if(frameno >= 15) {
		item = read_nurbs("bolla.db");
		set_attribute(item, bollicine_attr);
		set_translate(item, frame_pos_bolla[frameno - 15][0], frame_pos_bolla[frameno - 15][1], frame_pos_bolla[frameno - 15][2]);
		scene = insert_in_list(scene, item);
	}

	return scene;
}

ITEM build_knick_knack(unsigned frameno)
{
ITEM knick_knack, item;

	/*** Corpo ***/
	item = read_obj("corpo.obj", "Corpo");
	set_attribute(item, corpo_attr);
/*	set_bump_texture(item, attr, "textures/corpo_bump.hr", 0.0, 1.0, 0.0, 1.0, 0, 1.0, 1.0);*/
	knick_knack = create_list(item, "Knick_Knack");

	/*** Occhi ***/
	item = read_nurbs("occhio1.db");
	set_scale(item, frame_scala_occhi[frameno][0], frame_scala_occhi[frameno][1], frame_scala_occhi[frameno][2]);
	set_translate(item, 0.108, 0.583, 2.361);
	set_attribute(item, occhi_attr);
	knick_knack = insert_in_list(knick_knack, item);

	item = read_nurbs("occhio2.db");
	set_scale(item, frame_scala_occhi[frameno][0], frame_scala_occhi[frameno][1], frame_scala_occhi[frameno][2]);
	set_translate(item, -0.100, 0.569, 2.347);
	set_attribute(item, occhi_attr);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Bocca ***/
	item = read_obj("bocca.obj", "Bocca");
	set_attribute(item, bocca_attr);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Bottoni ***/
	item = read_obj("bottoni.obj", "Bottoni");
	set_attribute(item, bottoni_attr);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Sopracciglia ***/
	item = read_obj("sopracciglia.obj", "Sopracciglia");
	set_attribute(item, sopracciglia_attr);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Naso ***/
	item = read_nurbs("naso.db");
	set_attribute(item, naso_attr);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Cravatta ***/
	item = read_obj("cravatta.obj", "cravatta");
	set_attribute(item, cravatta_attr);
	set_z_rotate(item, 25.0, 1);
	knick_knack = insert_in_list(knick_knack, item);

	/*** Cappello ***/
	item = read_nurbs("cappello.db");
	set_attribute(item, cappello_attr);
	set_translate(item, 0.0, 0.0, 2.5);
	knick_knack = insert_in_list(knick_knack, item);

	return knick_knack;
}

ITEM build_cupola(unsigned frameno)
{
ITEM cupola, item;

	/*** Cupola base ***/
	item = read_nurbs("cupola_base_bordo.db");
	set_domain_texture(item, cupola_attr, "textures/cupola_base.hr", 0.0, 1.0, 0.0, 1.0, 0, 1.0, 1.0);
	cupola = create_list(item, "Cupola");

	item = read_nurbs("cupola_base_inf.db");
	set_attribute(item, cupola_attr);
	cupola = insert_in_list(cupola, item);

	item = read_nurbs("cupola_base_sup.db");
	set_attribute(item, cupola_attr);
	cupola = insert_in_list(cupola, item);

	/*** Cupola fronte ***/
	item = read_nurbs("cupola_fronte.dbe");
	set_attribute(item, cupola_fronte_attr);
	cupola = insert_in_list(cupola, item);

	/*** Cupola retro ***/
	item = read_nurbs("cupola_retro.db");
	set_domain_texture(item, cupola_retro_attr, "textures/cupola_retro.hr", 0.0, 1.0, 0.0, 1.0, 0, 1.0, 1.0);
	cupola = insert_in_list(cupola, item);

	return cupola;
}

ITEM build_igloo(unsigned frameno)
{
ITEM igloo, item;

	/*** Igloo fronte ***/
	item = read_nurbs("igloo_fronte.db");
	set_domain_texture(item, igloo_attr, "textures/igloo_fronte.hr", 0.0, 1.0, 0.0, 1.0, 0, 1.0, 1.0);
	igloo = create_list(item, "Igloo");

	/*** Igloo retro ***/
	item = read_nurbs("igloo_retro.db");
	set_domain_texture(item, igloo_attr, "textures/igloo_retro.hr", 0.0, 1.0, 0.0, 1.0, 0, 1.0, 1.0);
	igloo = insert_in_list(igloo, item);

	/*** Igloo bordo ***/
	item = read_nurbs("igloo_bordo.db");
	set_attribute(item, igloo_attr);
	igloo = insert_in_list(igloo, item);

	return igloo;
}

ITEM build_static_scene(void)
{
ITEM scene, item;

	/*** Pareti ***/
	item = read_nurbs("plane.db");
	set_domain_texture(item, sfondo_attr, "textures/pavimento.hr", 0.0, 0.25, 0.0, 0.25, 1, 1.0, 1.0);
	set_scale(item, 30.0, 30.0, 30.0);
	set_translate(item, 0.0, 0.0, -30.0);
	scene = create_list(item, "Ambiente");

	item = read_nurbs("plane.db");
	set_domain_texture(item, sfondo_attr, "textures/sfondo.hr", 0.0, 0.25, 0.0, 0.25, 1, 1.0, 1.0);
	set_y_rotate(item, 90.0, 1);
	set_x_rotate(item, 90.0, 1);
	set_scale(item, 30.0, 30.0, 30.0);
	set_translate(item, 30.0, 0.0, 0.0);
	scene = insert_in_list(scene, item);

	item = read_nurbs("plane.db");
	set_domain_texture(item, sfondo_attr, "textures/sfondo.hr", 0.0, 0.25, 0.0, 0.25, 1, 0.5, 1.0);
	set_scale(item, 30.0, 30.0, 30.0);
	set_x_rotate(item, 90.0, 1);
	set_translate(item, 0.0, -30.0, 0.0);
	scene = insert_in_list(scene, item);

	/*** Tavolo ***/
	item = read_obj("tavolo.obj", "Tavolo1");
	set_translate(item, -50.0, -50.0, -67.5);
	set_scale(item, 0.52, 0.5, 0.450450);
	set_domain_texture(item, tavolo_attr, "textures/wood3.hr", 0.0, 0.5, 0.0, 0.5, 1, 1.0, 1.0);
	scene = insert_in_list(scene, item);

	/*** Lampada ***/
	item = read_obj("lampada.obj", "Lampada1");
	set_z_rotate(item, 135.0, 1);
	set_translate(item, 85.0, 85.0, 0.0);
	set_scale(item, 0.1953125, 0.1953125, 0.1953125);
	set_attribute(item, lampada_attr);
	scene = insert_in_list(scene, item);

	return scene;
}

