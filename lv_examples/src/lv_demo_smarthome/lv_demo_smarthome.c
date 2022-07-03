/**
 * @file lv_demo_smarthome.c
 *
 */

#include "../../lv_examples.h"
#include "lv_demo_smarthome.h"


static void lv_demo_tabview(void);//标签栏
static void tabview1_create(lv_obj_t * parent);//标签页1
static void tabview2_create(lv_obj_t * parent);//标签页2
static void tabview3_create(lv_obj_t * parent);//标签页3
static void lv_smarthome_boot_animation(int time);//开机动画
static void night_dark_set(int mode);//夜间模式 or 日间模式
static void get_sys_time(void *arg);//获取系统时间
static void btn_event_all_led(lv_obj_t * obj, lv_event_t event);//声明按键(所有灯光)回调
static void btn_event_led1(lv_obj_t * obj, lv_event_t event);//声明按键(灯光1)回调

static lv_obj_t * background_animation;
static lv_obj_t * tabview;
static lv_obj_t * tabview1;
static lv_obj_t * tabview2;
static lv_obj_t * tabview3;
static lv_obj_t * btn_img1;//灯光1
static lv_obj_t * btn_img2;//灯光2
static lv_obj_t * btn_img3;//灯光3
static lv_obj_t * btn_img4;//所有灯光

static bool led_status = false;//LED标志位
static bool led_status1 = false;//LED1标志位


int year, month, date, hour, minute, second;


//声明按键回调
LV_EVENT_CB_DECLARE(event_handler)
{
    // switch (e)
    // {
    //     case LV_EVENT_CLICKED://轻点
    //         break;
    //     case LV_EVENT_RELEASED://按压
    //         break;
    //     case LV_EVENT_PRESSED://松开
    //         break;
    //     case LV_EVENT_LONG_PRESSED://长按
    //         break;
    //     default:break;
    // }
}


/**
 *  Main 
 * @param boot_time 开机时长，单位（秒）
 */
void lv_demo_smarthome(int boot_time)
{
    /* 开机动画 */
    lv_smarthome_boot_animation(boot_time);

    /* tabview */
    lv_demo_tabview();
    
    // 加载tabview屏幕,动画效果为LV_SCR_LOAD_ANIM_FADE_ON,切换时间为500ms,延迟 boot_time 后从第一屏开始切换，切换完成后删除开机动画屏幕
    lv_scr_load_anim(tabview, LV_SCR_LOAD_ANIM_FADE_ON, 500, boot_time*1000+100, true); 
}


/**
 * 标签栏
 */
static void lv_demo_tabview(void)
{
    tabview = lv_tabview_create(NULL, NULL);////创建一个tabview，但未加载到显示
    lv_tabview_set_btns_pos(tabview, LV_TABVIEW_TAB_POS_NONE);//隐藏tabview
    night_dark_set(0);//夜间模式

    tabview1 = lv_tabview_add_tab(tabview, "HOME");
    tabview2 = lv_tabview_add_tab(tabview, "LED");
    tabview3 = lv_tabview_add_tab(tabview, "MUSIC");

    /*Add content to the tabs*/
    // lv_obj_t * label = lv_label_create(tabview1, NULL);
    // lv_label_set_text(label, "This the first tab\n\n"
    //                          "If the content\n"
    //                          "of a tab\n"
    //                          "becomes too\n"
    //                          "longer\n"
    //                          "than the\n"
    //                          "container\n"
    //                          "then it\n"
    //                          "automatically\n"
    //                          "becomes\n"
    //                          "scrollable.\n"
    //                          "\n"
    //                          "\n"
    //                          "\n"
    //                          "Can you see it?");
    //
    // label = lv_label_create(tabview2, NULL);
    // lv_label_set_text(label, "Second tab");
    //
    // label = lv_label_create(tabview3, NULL);
    // lv_label_set_text(label, "Third tab");

    tabview1_create(tabview1);
    tabview2_create(tabview2);
    tabview3_create(tabview3);
}


/**
 * 标签页1: 天气预报、时钟
 */
static void tabview1_create(lv_obj_t * parent)
{
    //文本
    lv_obj_t * label = lv_label_create(tabview1, NULL);//创建标签对象
    lv_label_set_recolor(label, true);//文本重新着色
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);//标签在界面居中显示
    lv_label_set_text(label, "page1");//设置文本内容
    // get_sys_time(NULL);
    // lv_label_set_text_fmt(label, "%d:%d", hour, minute);   

    // lv_task_create(get_sys_time, 1000, LV_TASK_PRIO_LOW, NULL);  // 1秒任务
    
}


/**
 * 标签页2: 灯光控制
 */
static void tabview2_create(lv_obj_t * parent)
{
    //文本
    // lv_obj_t * label = lv_label_create(parent, NULL);//创建标签对象
    // lv_label_set_recolor(label, true);//文本重新着色
    // lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);//标签在界面居中显示
    // lv_label_set_text(label, "page2");//设置文本内容

    //图片按钮 灯光1
	LV_IMG_DECLARE(light1_on); //声明图片
    LV_IMG_DECLARE(light1_off); //声明图片
	btn_img1 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img1, LV_BTN_STATE_PRESSED, &light1_off);//按下状态图片
	lv_imgbtn_set_src(btn_img1, LV_BTN_STATE_RELEASED, &light1_on);//抬起状态图片
    lv_imgbtn_set_src(btn_img1, LV_BTN_STATE_CHECKED_RELEASED, &light1_off);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img1, LV_BTN_STATE_CHECKED_PRESSED, &light1_on);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img1, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img1);//设置翻转
	lv_obj_align(btn_img1, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 50);//左上角对齐
    lv_obj_set_event_cb(btn_img1, btn_event_led1);//设置按钮回调

    //图片按钮 灯光2
	LV_IMG_DECLARE(light2_on); //声明图片
    LV_IMG_DECLARE(light2_off); //声明图片
	btn_img2 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img2, LV_BTN_STATE_PRESSED, &light2_off);//按下状态图片
	lv_imgbtn_set_src(btn_img2, LV_BTN_STATE_RELEASED, &light2_on);//抬起状态图片
    lv_imgbtn_set_src(btn_img2, LV_BTN_STATE_CHECKED_RELEASED, &light2_off);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img2, LV_BTN_STATE_CHECKED_PRESSED, &light2_on);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img2, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img2);//设置翻转
	lv_obj_align(btn_img2, NULL, LV_ALIGN_IN_TOP_LEFT, 180, 50);//左上角对齐
    lv_obj_set_event_cb(btn_img2, event_handler);//设置按钮回调

    //图片按钮 灯光3
	LV_IMG_DECLARE(light3_on); //声明图片
    LV_IMG_DECLARE(light3_off); //声明图片
	btn_img3 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img3, LV_BTN_STATE_PRESSED, &light3_off);//按下状态图片
	lv_imgbtn_set_src(btn_img3, LV_BTN_STATE_RELEASED, &light3_on);//抬起状态图片
    lv_imgbtn_set_src(btn_img3, LV_BTN_STATE_CHECKED_RELEASED, &light3_off);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img3, LV_BTN_STATE_CHECKED_PRESSED, &light3_on);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img3, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img3);//设置翻转
	lv_obj_align(btn_img3, NULL, LV_ALIGN_IN_TOP_LEFT, 310, 50);//左上角对齐
    lv_obj_set_event_cb(btn_img3, event_handler);//设置按钮回调

    //图片按钮 灯光全开
	LV_IMG_DECLARE(light1_all); //声明图片
    LV_IMG_DECLARE(light2_all); //声明图片
	btn_img4 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img4, LV_BTN_STATE_PRESSED, &light2_all);//按下状态图片
	lv_imgbtn_set_src(btn_img4, LV_BTN_STATE_RELEASED, &light1_all);//抬起状态图片
    lv_imgbtn_set_src(btn_img4, LV_BTN_STATE_CHECKED_RELEASED, &light2_all);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img4, LV_BTN_STATE_CHECKED_PRESSED, &light1_all);//检测到按下状态图片 
    lv_imgbtn_set_checkable(btn_img4, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img4);//设置翻转
	lv_obj_align(btn_img4, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 180);//左上角对齐
    lv_obj_set_event_cb(btn_img4, btn_event_all_led);//设置按钮回调

    //图片按钮 空调
	LV_IMG_DECLARE(ac_on); //声明图片
    LV_IMG_DECLARE(ac_off); //声明图片
	lv_obj_t* btn_img5 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img5, LV_BTN_STATE_PRESSED, &ac_off);//按下状态图片
	lv_imgbtn_set_src(btn_img5, LV_BTN_STATE_RELEASED, &ac_on);//抬起状态图片
    lv_imgbtn_set_src(btn_img5, LV_BTN_STATE_CHECKED_RELEASED, &ac_off);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img5, LV_BTN_STATE_CHECKED_PRESSED, &ac_on);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img5, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img5);//设置翻转
	lv_obj_align(btn_img5, NULL, LV_ALIGN_IN_TOP_LEFT, 245, 180);//左上角对齐
    lv_obj_set_event_cb(btn_img5, event_handler);//设置按钮回调

    //图片按钮 回家模式
	LV_IMG_DECLARE(home); //声明图片
	lv_obj_t* btn_img6 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img6, LV_BTN_STATE_PRESSED, &home);//按下状态图片
	lv_imgbtn_set_src(btn_img6, LV_BTN_STATE_RELEASED, &home);//抬起状态图片
    lv_imgbtn_set_src(btn_img6, LV_BTN_STATE_CHECKED_RELEASED, &home);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img6, LV_BTN_STATE_CHECKED_PRESSED, &home);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img6, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img6);//设置翻转
	lv_obj_align(btn_img6, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 310);//左上角对齐
    lv_obj_set_event_cb(btn_img6, event_handler);//设置按钮回调

    //图片按钮 离家模式
	LV_IMG_DECLARE(company); //声明图片
	lv_obj_t* btn_img7 = lv_imgbtn_create(parent, NULL);//创建图片按键
	lv_imgbtn_set_src(btn_img7, LV_BTN_STATE_PRESSED, &company);//按下状态图片
	lv_imgbtn_set_src(btn_img7, LV_BTN_STATE_RELEASED, &company);//抬起状态图片
    lv_imgbtn_set_src(btn_img7, LV_BTN_STATE_CHECKED_RELEASED, &company);//检测到按松开状态图片
    lv_imgbtn_set_src(btn_img7, LV_BTN_STATE_CHECKED_PRESSED, &company);//检测到按下状态图片  
    lv_imgbtn_set_checkable(btn_img7, true);//使能按键图片翻转
    lv_imgbtn_toggle(btn_img7);//设置翻转
	lv_obj_align(btn_img7, NULL, LV_ALIGN_IN_TOP_LEFT, 245, 310);//左上角对齐
    lv_obj_set_event_cb(btn_img7, event_handler);//设置按钮回调
}


/**
 * 标签页3: 音乐播放器
 */
static void tabview3_create(lv_obj_t * parent)
{
    //文本
    lv_obj_t * label = lv_label_create(parent, NULL);//创建标签对象
    lv_label_set_recolor(label, true);//文本重新着色
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);//标签在界面居中显示
    lv_label_set_text(label, "page3");//设置文本内容
}


/**
 * 开机动画
 * @param time 开机时长，单位（秒）.
 */
static void lv_smarthome_boot_animation(int time)
{
    //背景
    background_animation = lv_obj_create(lv_scr_act(), NULL);//创建基本对象，如果为 NULL 将创建一个屏幕
	lv_obj_clean_style_list(background_animation, LV_OBJ_PART_MAIN);//清空对象风格
	lv_obj_set_style_local_bg_opa(background_animation, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_100);//设置颜色覆盖度100%，数值越低，颜色越透。
	lv_obj_set_style_local_bg_color(background_animation, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);//设置背景颜色为绿色
    lv_obj_set_size(background_animation, LV_HOR_RES, LV_VER_RES);//设置覆盖面积

    //进度条
    lv_obj_t* bar = lv_bar_create(lv_scr_act(), NULL);//创建进度条对象
    lv_obj_set_size(bar, 200, 15);//进度条在界面里的长宽
    lv_obj_align(bar, NULL, LV_ALIGN_CENTER, 0, 0);//进度条在界面居中显示
    lv_bar_set_range(bar, 1, 100);//进度条范围
    lv_bar_set_anim_time(bar, time*1000);//设置lv_bar进度条的动画时长
    lv_bar_set_value(bar, 100, LV_ANIM_ON);//进度条的值, LV_ANIM_ON 开启动画
    lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);//设置背景为白色
    lv_obj_set_style_local_bg_color(bar, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_ORANGE);//设置前景为橙色

    //文本
    lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);//创建标签对象
    lv_label_set_recolor(label, true);//文本重新着色
    lv_label_set_text(label, "#FFA500 Software is upgrading #");//设置文本内容
    lv_obj_align(label, bar, LV_ALIGN_CENTER, 0, -30);//让文本参照进度条对齐
}


/**
 * 夜间模式 or 日间模式
 * @param mode 0 夜间模式 1 日间模式
 */
static void night_dark_set(int mode)
{
    uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
    if(mode==0) flag = LV_THEME_MATERIAL_FLAG_DARK;
        LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
                flag,
                lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());
}


//获取系统时间
static void get_sys_time(void *arg)
{
    #include   <stdio.h> 
    #include   <time.h> 

    struct tm * time_out;
    long ts;

    ts = time(NULL);
    time_out = localtime(&ts);

    year = time_out-> tm_year+1900;//年
    month = time_out-> tm_mon+1;   //月
    date = time_out-> tm_mday;     //日
    hour = time_out-> tm_hour;     //时
    minute = time_out-> tm_min;    //分
    second = time_out-> tm_sec;    //秒
    printf("\n%d-%d-%d %d:%d:%d\n", year, month, date, hour, minute, second);

    //文本
    lv_obj_t * label = lv_label_create(tabview1, NULL);//创建标签对象
    lv_label_set_recolor(label, true);//文本重新着色
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);//标签在界面居中显示
    lv_label_set_text_fmt(label, "%02d:%02d", minute, second);   
}


//声明按键(所有灯光)回调
static void btn_event_all_led(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED)
	{
		//对象被按下时触发,每次按下时只触发一次        
        if (led_status == false)
        {
            //声明图片
            LV_IMG_DECLARE(light1_on); 
            LV_IMG_DECLARE(light2_on); 
            LV_IMG_DECLARE(light3_on);     

            //图片按钮 灯光1
            lv_imgbtn_set_checkable(btn_img1, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img1);//设置翻转
	        lv_obj_align(btn_img1, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 50);//左上角对齐
                
            //图片按钮 灯光2
            lv_imgbtn_set_checkable(btn_img2, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img2);//设置翻转
	        lv_obj_align(btn_img2, NULL, LV_ALIGN_IN_TOP_LEFT, 180, 50);//左上角对齐
                
            //图片按钮 灯光3
            lv_imgbtn_set_checkable(btn_img3, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img3);//设置翻转
	        lv_obj_align(btn_img3, NULL, LV_ALIGN_IN_TOP_LEFT, 310, 50);//左上角对齐

		    printf("\nled on\n");            
            led_status = true;
        }
        else
        {
            //声明图片
            LV_IMG_DECLARE(light1_off); 
            LV_IMG_DECLARE(light2_off); 
            LV_IMG_DECLARE(light3_off);   

            //图片按钮 灯光1
            lv_imgbtn_set_checkable(btn_img1, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img1);//设置翻转
            lv_obj_align(btn_img1, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 50);//左上角对齐

            //图片按钮 灯光1
            lv_imgbtn_set_checkable(btn_img2, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img2);//设置翻转
            lv_obj_align(btn_img2, NULL, LV_ALIGN_IN_TOP_LEFT, 180, 50);//左上角对齐

            //图片按钮 灯光1
            lv_imgbtn_set_checkable(btn_img3, true);//使能按键图片翻转
            lv_imgbtn_toggle(btn_img3);//设置翻转
            lv_obj_align(btn_img3, NULL, LV_ALIGN_IN_TOP_LEFT, 310, 50);//左上角对齐

		    printf("\nled off\n");  
            led_status = false;   
        }
	}
}


//声明按键(灯光1)回调
static void btn_event_led1(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED)
	{
		//对象被按下时触发,每次按下时只触发一次        
        if (led_status1 == false)
        {
		    printf("\nled1 on\n");            
            led_status1 = true;
        }
        else
        {
		    printf("\nled1 off\n");  
            led_status1 = false;   
        }
	}    
}