// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
#include "stdio.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_Chart1;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Label3;
lv_obj_t * ui_Label4;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Label6;
lv_obj_t * ui_Label7;
lv_obj_t * ui_Panel2;
lv_obj_t * ui_Label8;
lv_obj_t * ui_Label9;
lv_obj_t * ui_Label10;
lv_obj_t * ui_Panel3;
lv_obj_t * ui_Label12;
lv_obj_t * ui_Label13;
lv_obj_t * ui_Label14;
lv_obj_t * ui_Label15;
lv_obj_t * ui_Label11;
lv_obj_t * ui_Image6;
lv_obj_t * ui_Image1;
lv_obj_t * ui_Image2;
lv_obj_t * ui_Label16;
lv_obj_t * ui_Label17;
lv_obj_t * ui_Label18;
lv_obj_t * ui_Label19;
lv_obj_t * ui_Label20;
lv_obj_t * ui_Label21;
lv_obj_t * ui_Label22;
lv_obj_t * ui_Label23;
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Label25;
lv_obj_t * ui_lblMoving;
lv_obj_t * ui_btnMessageBox;
lv_obj_t * ui_TextArea1;
lv_obj_t * ui_Bar1;
lv_obj_t * ui_Slider1;
lv_obj_t * ui_Keyboard1;

 lv_obj_t * meter;
 lv_meter_indicator_t *indic;
 lv_obj_t * mbox1;
 lv_obj_t * ui_LabelValue;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

void ui_add_points_to_chart(void);
void ui_draw_meter(void);

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_Slider1(lv_event_t * e)
{
  static char tmp_str[6];
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if(event == LV_EVENT_VALUE_CHANGED) 
  {
    _ui_bar_set_property(ui_Bar1, _UI_BAR_PROPERTY_VALUE, lv_slider_get_value(ta));
    lv_meter_set_indicator_value(meter, indic, lv_slider_get_value(ta));
    sprintf(tmp_str, "%d%%", lv_slider_get_value(ta));
    lv_label_set_text(ui_LabelValue, tmp_str);
  }
}

static void btnMsgBoxventHandler(lv_event_t * e)
{
  static const char * btns[] ={"Apply", "Close", ""};
  mbox1 = lv_msgbox_create(NULL, "Hello", "This is a message box with two buttons.", btns, true);
  //lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_center(mbox1);
}

static void btnTextBoxHandler(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if ((code == LV_EVENT_DEFOCUSED) || (code == LV_EVENT_READY))
  {
    lv_keyboard_set_textarea(ui_Keyboard1, NULL);
    lv_obj_add_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(ui_TextArea1, LV_STATE_FOCUSED);
  }
  else if (code == LV_EVENT_FOCUSED)
  {
    lv_keyboard_set_textarea(ui_Keyboard1, ui_TextArea1);
    lv_obj_clear_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);
  }
    
}

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{

    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Label1

    ui_Label1 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label1, -187);
    lv_obj_set_y(ui_Label1, -212);

    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label1, "Weather Station Demo");

    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0x8080FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel1

    ui_Panel1 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel1, 206);
    lv_obj_set_height(ui_Panel1, 141);

    lv_obj_set_x(ui_Panel1, 194);
    lv_obj_set_y(ui_Panel1, -81);

    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Chart1

    ui_Chart1 = lv_chart_create(ui_Screen1);

    lv_obj_set_width(ui_Chart1, 360);
    lv_obj_set_height(ui_Chart1, 161);

    lv_obj_set_x(ui_Chart1, -98);
    lv_obj_set_y(ui_Chart1, -71);

    lv_obj_set_align(ui_Chart1, LV_ALIGN_CENTER);

    // ui_Label2

    ui_Label2 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label2, 215);
    lv_obj_set_y(ui_Label2, -173);

    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label2, "1 June 2022 Wed");

    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label3

    ui_Label3 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label3, 141);
    lv_obj_set_y(ui_Label3, -83);

    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label3, "Outside:");

    // ui_Label4

    ui_Label4 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label4, 229);
    lv_obj_set_y(ui_Label4, -85);

    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label4, "25.4 C");

    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label5

    ui_Label5 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label5, 198);
    lv_obj_set_y(ui_Label5, -122);

    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label5, "Temperature");

    lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label5, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label6

    ui_Label6 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label6, 149);
    lv_obj_set_y(ui_Label6, -43);

    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label6, "Inside:");

    // ui_Label7

    ui_Label7 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label7, 233);
    lv_obj_set_y(ui_Label7, -46);

    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label7, "23.1 C");

    lv_obj_set_style_text_font(ui_Label7, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel2

    ui_Panel2 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel2, 206);
    lv_obj_set_height(ui_Panel2, 88);

    lv_obj_set_x(ui_Panel2, 197);
    lv_obj_set_y(ui_Panel2, 50);

    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Label8

    ui_Label8 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label8, 199);
    lv_obj_set_y(ui_Label8, 27);

    lv_obj_set_align(ui_Label8, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label8, "Humidity");

    lv_obj_set_style_text_color(ui_Label8, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label8, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label8, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label9

    ui_Label9 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label9, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label9, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label9, 153);
    lv_obj_set_y(ui_Label9, 65);

    lv_obj_set_align(ui_Label9, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label9, "Inside:");

    // ui_Label10

    ui_Label10 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label10, 228);
    lv_obj_set_y(ui_Label10, 63);

    lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label10, "42%");

    lv_obj_set_style_text_font(ui_Label10, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel3

    ui_Panel3 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel3, 206);
    lv_obj_set_height(ui_Panel3, 80);

    lv_obj_set_x(ui_Panel3, 200);
    lv_obj_set_y(ui_Panel3, 150);

    lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Label12

    ui_Label12 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label12, 200);
    lv_obj_set_y(ui_Label12, 133);

    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label12, "CO2");

    lv_obj_set_style_text_color(ui_Label12, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label12, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label12, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label13

    ui_Label13 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label13, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label13, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label13, 134);
    lv_obj_set_y(ui_Label13, 164);

    lv_obj_set_align(ui_Label13, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label13, "Inside:");

    // ui_Label14

    ui_Label14 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label14, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label14, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label14, 199);
    lv_obj_set_y(ui_Label14, 163);

    lv_obj_set_align(ui_Label14, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label14, "1340");

    lv_obj_set_style_text_color(ui_Label14, lv_color_hex(0xBE0808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label14, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label15

    ui_Label15 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label15, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label15, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label15, 257);
    lv_obj_set_y(ui_Label15, 166);

    lv_obj_set_align(ui_Label15, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label15, "PPM");

    // ui_Label11

    ui_Label11 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label11, -253);
    lv_obj_set_y(ui_Label11, 52);

    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label11, "Forecast");

    lv_obj_set_style_text_color(ui_Label11, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label11, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label11, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Image6

    ui_Image6 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image6, &ui_img_rain_png);

    lv_obj_set_width(ui_Image6, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image6, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image6, -1);
    lv_obj_set_y(ui_Image6, 125);

    lv_obj_set_align(ui_Image6, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Image6, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image6, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Image1

    ui_Image1 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image1, &ui_img_few_clouds_png);

    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image1, -130);
    lv_obj_set_y(ui_Image1, 123);

    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Image2

    ui_Image2 = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Image2, &ui_img_broken_clouds_png);

    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image2, -254);
    lv_obj_set_y(ui_Image2, 121);

    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Label16

    ui_Label16 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label16, -258);
    lv_obj_set_y(ui_Label16, 76);

    lv_obj_set_align(ui_Label16, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label16, "Wed");

    // ui_Label17

    ui_Label17 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label17, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label17, -134);
    lv_obj_set_y(ui_Label17, 78);

    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label17, "Thu");

    // ui_Label18

    ui_Label18 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label18, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label18, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label18, -2);
    lv_obj_set_y(ui_Label18, 76);

    lv_obj_set_align(ui_Label18, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label18, "Fri");

    // ui_Label19

    ui_Label19 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label19, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label19, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label19, -253);
    lv_obj_set_y(ui_Label19, 174);

    lv_obj_set_align(ui_Label19, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label19, "25 C");

    lv_obj_set_style_text_font(ui_Label19, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label20

    ui_Label20 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label20, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label20, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label20, -127);
    lv_obj_set_y(ui_Label20, 174);

    lv_obj_set_align(ui_Label20, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label20, "22 C");

    lv_obj_set_style_text_font(ui_Label20, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label21

    ui_Label21 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label21, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label21, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label21, -4);
    lv_obj_set_y(ui_Label21, 176);

    lv_obj_set_align(ui_Label21, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label21, "18 C");

    lv_obj_set_style_text_font(ui_Label21, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label22

    ui_Label22 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label22, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label22, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label22, 268);
    lv_obj_set_y(ui_Label22, -205);

    lv_obj_set_align(ui_Label22, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label22, "15:23");

    lv_obj_set_style_text_font(ui_Label22, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Label23

    ui_Label23 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_Label23, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label23, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label23, -208);
    lv_obj_set_y(ui_Label23, -170);

    lv_obj_set_align(ui_Label23, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label23, "Air Pressure");

    lv_obj_set_style_text_color(ui_Label23, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label23, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label23, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_Screen2_screen_init(void)
{

    // ui_Screen2

    ui_Screen2 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);
    
    // ui_Label25

    ui_Label25 = lv_label_create(ui_Screen2);

    lv_obj_set_width(ui_Label25, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label25, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Label25, -218);
    lv_obj_set_y(ui_Label25, -208);

    lv_obj_set_align(ui_Label25, LV_ALIGN_CENTER);

    lv_label_set_text(ui_Label25, "Actions Demo");

    lv_obj_set_style_text_color(ui_Label25, lv_color_hex(0x8080FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label25, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label25, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    

    // ui_lblMoving

    ui_lblMoving = lv_label_create(ui_Screen2);

    lv_obj_set_width(ui_lblMoving, 150);
    lv_obj_set_height(ui_lblMoving, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_lblMoving, -221);
    lv_obj_set_y(ui_lblMoving, -173);

    lv_obj_set_align(ui_lblMoving, LV_ALIGN_CENTER);

    lv_label_set_text(ui_lblMoving, "IT IS MOVING TEXT");
    lv_label_set_long_mode(ui_lblMoving, LV_LABEL_LONG_SCROLL_CIRCULAR); //Circular scroll
    lv_obj_set_style_text_font(ui_lblMoving, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    

    // ui_btnMessageBox

    ui_btnMessageBox = lv_btn_create(ui_Screen2);

    lv_obj_set_width(ui_btnMessageBox, 132);
    lv_obj_set_height(ui_btnMessageBox, 39);

    lv_obj_set_x(ui_btnMessageBox, 240);
    lv_obj_set_y(ui_btnMessageBox, -162);

    lv_obj_set_align(ui_btnMessageBox, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_btnMessageBox, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_btnMessageBox, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(ui_btnMessageBox, btnMsgBoxventHandler, LV_EVENT_PRESSED, NULL);
    
    lv_obj_t *lblMsgBoxBtn = lv_label_create(ui_btnMessageBox);
    lv_label_set_text(lblMsgBoxBtn, "Msg Box");
    lv_obj_set_align(lblMsgBoxBtn, LV_ALIGN_CENTER);
    
    
    // ui_TextArea1

    ui_TextArea1 = lv_textarea_create(ui_Screen2);

    lv_obj_set_width(ui_TextArea1, 226);
    lv_obj_set_height(ui_TextArea1, 92);

    lv_obj_set_x(ui_TextArea1, -188);
    lv_obj_set_y(ui_TextArea1, -99);

    lv_obj_set_align(ui_TextArea1, LV_ALIGN_CENTER);

    if("" == "") 
      lv_textarea_set_accepted_chars(ui_TextArea1, NULL);
    else 
      lv_textarea_set_accepted_chars(ui_TextArea1, "");

    lv_textarea_set_text(ui_TextArea1, "");
    lv_textarea_set_placeholder_text(ui_TextArea1, "Enter text here");
    

    // ui_Bar1
    ui_Bar1 = lv_bar_create(ui_Screen2);
    lv_bar_set_range(ui_Bar1, 0, 100);
    lv_bar_set_value(ui_Bar1, 25, LV_ANIM_OFF);

    lv_obj_set_width(ui_Bar1, 211);
    lv_obj_set_height(ui_Bar1, 17);

    lv_obj_set_x(ui_Bar1, -189);
    lv_obj_set_y(ui_Bar1, -32);

    lv_obj_set_align(ui_Bar1, LV_ALIGN_CENTER);

    //lv_obj_set_style_bg_color(ui_Bar1, lv_color_hex(0x00BAFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_opa(ui_Bar1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(ui_Bar1, lv_color_hex(0xFCFF00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_opa(ui_Bar1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // ui_Slider1

    ui_Slider1 = lv_slider_create(ui_Screen2);
    lv_slider_set_range(ui_Slider1, 0, 100);

    lv_obj_set_width(ui_Slider1, 195);
    lv_obj_set_height(ui_Slider1, 10);

    lv_obj_set_x(ui_Slider1, -188);
    lv_obj_set_y(ui_Slider1, 5);

    lv_obj_set_align(ui_Slider1, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_Slider1, ui_event_Slider1, LV_EVENT_ALL, NULL);

    // ui_Keyboard1

    ui_Keyboard1 = lv_keyboard_create(ui_Screen2);

    lv_obj_set_width(ui_Keyboard1, 626);
    lv_obj_set_height(ui_Keyboard1, 189);

    lv_obj_set_x(ui_Keyboard1, -1);
    lv_obj_set_y(ui_Keyboard1, 140);

    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);
    
    lv_obj_add_event_cb(ui_TextArea1, btnTextBoxHandler, LV_EVENT_ALL, ui_Keyboard1);

    // POST CALLS
    lv_keyboard_set_textarea(ui_Keyboard1, ui_TextArea1);
    lv_obj_clear_flag(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN);
    
    ui_LabelValue = lv_label_create(ui_Screen2);
    lv_obj_set_width(ui_LabelValue, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelValue, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelValue, 236);
    lv_obj_set_y(ui_LabelValue, -102);
    lv_obj_set_align(ui_LabelValue, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelValue, "0%");
    lv_obj_set_style_text_font(ui_LabelValue, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    ui_Screen1_screen_init();
    ui_add_points_to_chart();
    lv_disp_load_scr(ui_Screen1);
}

void ui_add_points_to_chart(void)
{
  lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 730, 790);
  lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 0, 7);
  lv_chart_set_point_count(ui_Chart1, 8);
  
  //(obj, axis, major_len, minor_len,   major_cnt, minor_cnt, label_en, draw_size)
  lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5,  7, 2, true, 30);
  lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 8, 1, true, 40);
  
  lv_chart_set_div_line_count(ui_Chart1, 3, 8);
  
  lv_chart_series_t * ser1 = lv_chart_add_series(
    ui_Chart1, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  
  lv_chart_set_next_value(ui_Chart1, ser1, 765);
  lv_chart_set_next_value(ui_Chart1, ser1, 780);
  lv_chart_set_next_value(ui_Chart1, ser1, 775);
  lv_chart_set_next_value(ui_Chart1, ser1, 770);
  lv_chart_set_next_value(ui_Chart1, ser1, 773);
  lv_chart_set_next_value(ui_Chart1, ser1, 758);
  lv_chart_set_next_value(ui_Chart1, ser1, 750);
  lv_chart_set_next_value(ui_Chart1, ser1, 755);
  
  lv_chart_refresh(ui_Chart1);
}

void ui_draw_meter(void)
{
  meter = lv_meter_create(ui_Screen2);
  lv_obj_set_size(meter, 200, 200);
  lv_obj_set_align(meter, LV_ALIGN_CENTER);
  lv_obj_set_x(meter, 43);
  lv_obj_set_y(meter, -78);
  
  /*Add a scale first*/
  lv_meter_scale_t * scale = lv_meter_add_scale(meter);
  lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);
  
  /*Add a blue arc to the start*/
  indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_meter_set_indicator_start_value(meter, indic, 0);
  lv_meter_set_indicator_end_value(meter, indic, 20);
  
  /*Make the tick lines blue at the start of the scale*/
  indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE),
                                   false, 0);
  lv_meter_set_indicator_start_value(meter, indic, 0);
  lv_meter_set_indicator_end_value(meter, indic, 20);
  
  /*Add a red arc to the end*/
  indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(meter, indic, 80);
  lv_meter_set_indicator_end_value(meter, indic, 100);
  
  /*Make the tick lines red at the end of the scale*/
  indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false,
                                   0);
  lv_meter_set_indicator_start_value(meter, indic, 80);
  lv_meter_set_indicator_end_value(meter, indic, 100);
  
  /*Add a needle line indicator*/
  indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
  
  lv_meter_set_indicator_value(meter, indic, 45);
}

void ui_switch_screeen(void)
{
  lv_obj_del(ui_Screen1);
  ui_Screen2_screen_init();
  ui_draw_meter();
  lv_disp_load_scr(ui_Screen2);
}

