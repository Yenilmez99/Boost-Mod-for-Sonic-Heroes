using System.ComponentModel;
using Boost_Mod_for_Sonic_Heroes.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;
using System.ComponentModel.DataAnnotations;

namespace Boost_Mod_for_Sonic_Heroes.Configuration;

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
}

/// <summary>
/// Allows you to override certain aspects of the configuration creation process (e.g. create multiple configurations).
/// Override elements in <see cref="ConfiguratorMixinBase"/> for finer control.
/// </summary>
public class ConfiguratorMixin : ConfiguratorMixinBase
{
    // 
}
