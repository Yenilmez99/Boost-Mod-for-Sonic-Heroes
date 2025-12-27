using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using Boost_For_Sonic_Heroes.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;

namespace Boost_For_Sonic_Heroes.Configuration
{
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


        [DisplayName("Max Speed")]
        [Description("The limit of the maximum speed that the Boost feature adds to the character.")]
        [DefaultValue(15.0f)]
        public float Maximum_Velocity { get; set; } = 15.0f;


        [DisplayName("In How Many Second")]
        [Description("How many seconds does it take to reach maximum speed?")]
        [DefaultValue(2.0f)]
        public float In_How_Many_Second { get; set; } = 2.0f;

    }

    /// <summary>
    /// Allows you to override certain aspects of the configuration creation process (e.g. create multiple configurations).
    /// Override elements in <see cref="ConfiguratorMixinBase"/> for finer control.
    /// </summary>
    public class ConfiguratorMixin : ConfiguratorMixinBase
    {
        // 
    }
}
