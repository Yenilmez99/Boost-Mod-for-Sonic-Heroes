#if (IncludeConfig)
using System.ComponentModel;
using Reloaded.Mod.Template.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;
using System.ComponentModel.DataAnnotations;

namespace Reloaded.Mod.Template.Configuration;

public class Config : Configurable<Config>
{
    
    [DisplayName("Boost for All")]
    [Description("It allows you to use boosts with all characters.")]
    [DefaultValue(true)]
    public bool Boost_For_All { get; set; } = true;


    [DisplayName("Limited Boost")]
    [Description("The boost bar becomes active. Boost is consumed from the Team Blast bar.")]
    [DefaultValue(false)]
    public bool Limited_Boost { get; set; } = false;
        
    [DisplayName("Boost Rings")]
    [Description("Visual feedback when Boost is active.")]
    [DefaultValue(false)]
    public bool Boost_Rings { get; set; } = false;


    [DisplayName("Max Speed")]
    [Description("The limit of the maximum speed that the Boost feature adds to the character.")]
    [DefaultValue(15.0f)]
    public float Maximum_Velocity { get; set; } = 15.0f;


    [DisplayName("In How Many Second")]
    [Description("How many seconds does it take to reach maximum speed?")]
    [DefaultValue(2.0f)]
    public float In_How_Many_Second { get; set; } = 2.0f;

        
    [DisplayName("Boost Rings Color Intensity")]
    [Description("Color Saturation of Boost Rings (high values ​​may cause a crash)")]
    [DefaultValue(5)]
    public int Boost_Rings_Color_Intensity { get; set; } = 5;
        

    [DisplayName("Boost Rings Frequency")]
    [Description("Sets how often Boost Rings appear, in frames (1 second = 60 frames in Sonic Heroes, low values ​​may cause a crash\nIt must be more than 3).")]
    [DefaultValue(5)]
    public int Boost_Rings_Frequency { get; set; } = 5;
    

    [DisplayName("Controller Key")]
    [Description("The Xbox 360 controller was used as a reference.")]
    [DefaultValue(SampleEnum.XBOX360_Controller_Key_RB)]
    public SampleEnum Boost_Controller_Key { get; set; } = SampleEnum.XBOX360_Controller_Key_RB;

    public enum SampleEnum
    {
        [Display(Name = "A")]
        XBOX360_Controller_Key_A,
        [Display(Name = "B")]
        XBOX360_Controller_Key_B,
        [Display(Name = "X")]
        XBOX360_Controller_Key_X,
        [Display(Name = "Y")]
        XBOX360_Controller_Key_Y,
        [Display(Name = "LB (L1)")]
        XBOX360_Controller_Key_LB,
        [Display(Name = "RB (R1)")]
        XBOX360_Controller_Key_RB,
        [Display(Name = "BACK")]
        XBOX360_Controller_Key_BACK,
        [Display(Name = "START")]
        XBOX360_Controller_Key_START,
        [Display(Name = "LS (L3)")]
        XBOX360_Controller_Key_LS,
        [Display(Name = "RS (R3)")]
        XBOX360_Controller_Key_RS,
    }
}

/// <summary>
/// Allows you to override certain aspects of the configuration creation process (e.g. create multiple configurations).
/// Override elements in <see cref="ConfiguratorMixinBase"/> for finer control.
/// </summary>
public class ConfiguratorMixin : ConfiguratorMixinBase
{
    // 
}
#endif