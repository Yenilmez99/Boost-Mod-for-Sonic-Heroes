using System.ComponentModel;
using Boost_For_All_Mod_Config.Template.Configuration;
using Reloaded.Mod.Interfaces.Structs;

namespace Boost_For_All_Mod_Config.Configuration
{
    public class Config : Configurable<Config>
    {
        /*
            User Properties:
                - Please put all of your configurable properties here.
    
            By default, configuration saves as "Config.json" in mod user config folder.    
            Need more config files/classes? See Configuration.cs
    
            Available Attributes:
            - Category
            - DisplayName
            - Description
            - DefaultValue

            // Technically Supported but not Useful
            - Browsable
            - Localizable

            The `DefaultValue` attribute is used as part of the `Reset` button in Reloaded-Launcher.
        */

        [DisplayName("Controller Key")]
        [Description("Keys:\nA Key: 1\nB Key: 2\nX Key: 3\nY Key: 4\nL1: 5\nR1: 6\nBack Button: 7\nStart Button: 8\nL3: 9\nR3: 10\nL2: 11\nR2: 12(Default: 12)")]
        [DefaultValue(12)]
        public int Integer { get; set; } = 12;

        [DisplayName("Max Speed")]
        [Description("(1 - 120, Default 20)")]
        [DefaultValue(20.0f)]
        public float Float { get; set; } = 20.0f;

        [DisplayName("Time To Max Speed")]
        [Description("(1 - 10, Default 2)")]
        [DefaultValue(2.0f)]
        public float Float2 { get; set; } = 2.0f;

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
