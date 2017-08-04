within ;
package SolarThermoPro
                       extends Modelica.Icons.Package;
  package Base
               extends Modelica.Icons.BasesPackage;

    partial model TPBase

      // Import
      import Modelica.SIunits;
      import Modelica.Media;
      import Cv = Modelica.SIunits.Conversions;

      // Outputs
      output Cv.NonSIunits.Temperature_degC TdegC;
      output SIunits.Density d;
      output SIunits.SpecificEnthalpy h;
      output SIunits.DynamicViscosity v;
      output SIunits.ThermalConductivity k;
      output SIunits.SpecificHeatCapacity cp;
      //output SIunits.SpecificEntropy s;
      //output SIunits.SpecificInternalEnergy u;
      //output SIunits.SpecificHeatCapacity cv;

      // Parameters
      parameter Cv.NonSIunits.Temperature_degC TdegC_min;
      parameter Cv.NonSIunits.Temperature_degC TdegC_max;
      parameter Real nullValue = 0;

    protected
      Cv.NonSIunits.Pressure_bar     pbar;
      SIunits.Temperature            T = Cv.from_degC(TdegC);
      SIunits.Pressure               p = Cv.from_bar(pbar);

      annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(coordinateSystem(
              preserveAspectRatio=false)));
    end TPBase;

    partial model TPBaseMedia
      extends TPBase;

      import Modelica.Media;

      // Partial medium
      replaceable package medium = Media.Interfaces.PartialMedium constrainedby
        Media.Interfaces.PartialMedium;

    protected
      medium.ThermodynamicState state;

    equation
      // Thermodynamic state
      state.T = T;
      state.p = p;
      // Thermodynamic properties
      if TdegC_min > TdegC_max or (TdegC>=TdegC_min and TdegC<=TdegC_max) then
        cp =  medium.specificHeatCapacityCp(state);
        k  =  medium.thermalConductivity(state);
        d  =  medium.density(state);
        h  =  medium.specificEnthalpy(state);
        v  =  medium.dynamicViscosity(state);
      else
        cp = nullValue;
        k  = nullValue;
        d  = nullValue;
        h  = nullValue;
        v  = nullValue;
      end if;


      annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(coordinateSystem(preserveAspectRatio=false)));
    end TPBaseMedia;

    partial model TPBaseMediaIncompressible
      extends TPBaseMedia;

      import Modelica.SIunits.Conversions;

      // Parameters
      parameter Conversions.NonSIunits.Temperature_degC TdegC_start = 100 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC TdegC_stop =  300 "Final temperature (ºC)";

    equation

      // Temperature and pressure
      TdegC = TdegC_start + (TdegC_stop - TdegC_start)*time;
      pbar  = 0;

      annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(coordinateSystem(preserveAspectRatio=false)));
    end TPBaseMediaIncompressible;
  end Base;

  package Media
                extends Modelica.Icons.Package;
    package ThermalOil
                       extends Modelica.Icons.Package;

      package Therminol55 "Therminol 55 medium"
        import Modelica.Media;
        import Modelica.SIunits.Conversions;
        extends Media.Incompressible.TableBased(
          mediumName="Therminol 55",
          T_min = Conversions.from_degC(-29),
          T_max = Conversions.from_degC(316),
          TinK = false,
          T0=Conversions.from_degC(-18),
          tableDensity=
            [-29, 905;
             -26, 903;
             -18, 897;
              -7, 890;
               4, 882;
              16, 875;
              27, 867;
              38, 860;
              49, 852;
              60, 845;
              71, 837;
              82, 830;
              93, 822;
             104, 815;
             116, 807;
             127, 800;
             138, 792;
             149, 784;
             160, 777;
             171, 769;
             182, 761;
             193, 753;
             204, 745;
             216, 737;
             227, 729;
             238, 721;
             249, 712;
             260, 704;
             271, 695;
             282, 686;
             288, 682;
             293, 677;
             304, 668;
             316, 659],
          tableHeatCapacity=
            [-29, 1.73 * 10^3;
             -26, 1.74 * 10^3;
             -18, 1.77 * 10^3;
              -7, 1.81 * 10^3;
               4, 1.85 * 10^3;
              16, 1.89 * 10^3;
              27, 1.93 * 10^3;
              38, 1.97 * 10^3;
              49, 2.01 * 10^3;
              60, 2.05 * 10^3;
              71, 2.09 * 10^3;
              82, 2.13 * 10^3;
              93, 2.17 * 10^3;
             104, 2.21 * 10^3;
             116, 2.25 * 10^3;
             127, 2.29 * 10^3;
             138, 2.31 * 10^3;
             149, 2.36 * 10^3;
             160, 2.40 * 10^3;
             171, 2.44 * 10^3;
             182, 2.48 * 10^3;
             193, 2.52 * 10^3;
             204, 2.56 * 10^3;
             216, 2.60 * 10^3;
             227, 2.64 * 10^3;
             238, 2.68 * 10^3;
             249, 2.72 * 10^3;
             260, 2.75 * 10^3;
             271, 2.79 * 10^3;
             282, 2.83 * 10^3;
             288, 2.85 * 10^3;
             293, 2.87 * 10^3;
             304, 2.91 * 10^3;
             316, 2.95 * 10^3],
          tableConductivity=
            [-29, 0.1341;
             -26, 0.1337;
             -18, 0.1328;
              -7, 0.1315;
               4, 0.1302;
              16, 0.1289;
              27, 0.1276;
              38, 0.1264;
              49, 0.1251;
              60, 0.1238;
              71, 0.1225;
              82, 0.1212;
              93, 0.1199;
             104, 0.1186;
             116, 0.1173;
             127, 0.1160;
             138, 0.1147;
             149, 0.1134;
             160, 0.1121;
             171, 0.1108;
             182, 0.1095;
             193, 0.1082;
             204, 0.1069;
             216, 0.1056;
             227, 0.1043;
             238, 0.1030;
             249, 0.1017;
             260, 0.1003;
             271, 0.0990;
             282, 0.0997;
             288, 0.0971;
             293, 0.0964;
             304, 0.0951;
             316, 0.0938],
          tableViscosity=
            [-29, 1900 * 10^(-3);
             -26, 1405 * 10^(-3);
             -18, 612 * 10^(-3);
              -7, 236 * 10^(-3);
               4, 105.4 * 10^(-3);
              16, 53.1 * 10^(-3);
              27, 29.6 * 10^(-3);
              38, 17.87 * 10^(-3);
              49, 11.57 * 10^(-3);
              60, 7.93 * 10^(-3);
              71, 5.71 * 10^(-3);
              82, 4.27 * 10^(-3);
              93, 3.31 * 10^(-3);
             104, 2.64 * 10^(-3);
             116, 2.16 * 10^(-3);
             127, 1.797 * 10^(-3);
             138, 1.524 * 10^(-3);
             149, 1.311 * 10^(-3);
             160, 1.142 * 10^(-3);
             171, 1.005 * 10^(-3);
             182, 0.892 * 10^(-3);
             193, 0.798 * 10^(-3);
             204, 0.718 * 10^(-3);
             216, 0.650 * 10^(-3);
             227, 0.590 * 10^(-3);
             238, 0.538 * 10^(-3);
             249, 0.492 * 10^(-3);
             260, 0.451 * 10^(-3);
             271, 0.414 * 10^(-3);
             282, 0.381 * 10^(-3);
             288, 0.366 * 10^(-3);
             293, 0.351 * 10^(-3);
             304, 0.324 * 10^(-3);
             316, 0.298 * 10^(-3)],
          tableVaporPressure=
            [ 82, 0.012 * 10^3;
              93, 0.023 * 10^3;
             104, 0.041 * 10^3;
             116, 0.071 * 10^3;
             127, 0.12 * 10^3;
             138, 0.20 * 10^3;
             149, 0.32 * 10^3;
             160, 0.50 * 10^3;
             171, 0.77 * 10^3;
             182, 1.16 * 10^3;
             193, 1.71 * 10^3;
             204, 2.49 * 10^3;
             216, 3.55 * 10^3;
             227, 4.99 * 10^3;
             238, 6.92 * 10^3;
             249, 9.47 * 10^3;
             260, 12.8 * 10^3;
             271, 17.1 * 10^3;
             282, 22.5 * 10^3;
             288, 25.8 * 10^3;
             293, 29.4 * 10^3;
             304, 38.0 * 10^3;
             316, 48.7 * 10^3]);
        constant Real[:,2] tableHeatOfVaporization=
            [-29, 418.3 * 10^3;
             -26, 416.3 * 10^3;
             -18, 410.5 * 10^3;
              -7, 402.7 * 10^3;
               4, 395.1 * 10^3;
              16, 387.5 * 10^3;
              27, 380.1 * 10^3;
              38, 372.8 * 10^3;
              49, 365.6 * 10^3;
              60, 358.5 * 10^3;
              71, 351.5 * 10^3;
              82, 344.6 * 10^3;
              93, 337.8 * 10^3;
             104, 331.1 * 10^3;
             116, 324.5 * 10^3;
             127, 317.9 * 10^3;
             138, 311.5 * 10^3;
             149, 305.1 * 10^3;
             160, 298.8 * 10^3;
             171, 292.6 * 10^3;
             182, 286.5 * 10^3;
             193, 280.3 * 10^3;
             204, 274.3 * 10^3;
             216, 268.3 * 10^3;
             227, 262.3 * 10^3;
             238, 256.3 * 10^3;
             249, 250.3 * 10^3;
             260, 244.3 * 10^3;
             271, 238.3 * 10^3;
             282, 232.3 * 10^3;
             288, 229.3 * 10^3;
             293, 226.3 * 10^3;
             304, 220.1 * 10^3;
             316, 214.0 * 10^3] "Table for heat of vaporization";

          annotation (Documentation(info="<html>
<p><h4>Therminol 55 Heat Transfer Fluid</h4></p>
<p><br/><b><a href=\"https://team.solutia.com/sites/msds/Therminol%20MSDS%20Documents/196WEN.pdf\">View MSDS</a></b></p>
<p><b><a href=\"http://www.therminol.com/pages/contact/regform.asp?dir=bulletins&page=default.asp&where=55\">Product Bulletin</a></b> </p>
<p><br/><b>Product description</b></p>
<table cellspacing=\"0\" cellpadding=\"2\" border=\"0\"><tr>
<td><p><br/>Therminol 55 is a synthetic heat transfer fluid used in moderate temperature applications (-15&deg; F to 550&deg; F or -25&deg;C to 290&deg;C). Therminol 55 delivers a number of important performance benefits: </p></td>
</tr>
</table>
<p><ul>
<li><b>Long Life</b> - Therminol 55 is a true 550&deg; F (290&deg; C) fluid. You will get years of reliable, cost effective performance, even when operating your system <u>continuously</u> at 550&deg; F (290&deg; C). This means you do not have to over specify your fluid. </li>
<li><b>Non Fouling</b> - Because Therminol 55 is a synthetic fluid, it resists the effects of oxidation 10 times better than mineral oils. Less oxidation means less solids formation and much less fouling. For systems without nitrogen inerting, the performance advantages are significant. </li>
<li><b>Superb Low Temperature Pumpability</b> - Therminol 55 is still pumpable at -15&deg; F (-25&deg; C), long after mineral oils have become jelly-like. In fact, some mineral oils will not pump at temperatures below 20&deg; F (-7&deg; C). With Therminol 55, your heat transfer fluid system can start-up quickly and easily. </li>
<li><b>Low Cost</b> - This list of performance features does not cost a fortune. Call to see how affordable Therminol 55 can be. </li>
</ul></p>
<p><h4>Applications</h4></p>
<p>Therminol 55 is used in a wide variety of industries, such as: </p>
<p><ul>
<li>Natural Gas purification </li>
<li>Plastics/molding operations </li>
<li>Asphalt and Petroleum Product Storage </li>
<li>Biodiesel </li>
</ul></p>
<p>For more information on Therminol 55 systems and applications <a href=\"mailto:team.therminol@solutia.com\">e-mail</a> us or contact the Therminol Technical Service Hotline at <b>1-800-433-6997</b>. </p>
<p><h4>Detail properties</h4></p>
<p><a href=\"http://www.therminol.com/pages/contact/regform.asp?dir=bulletins&page=default.asp&where=55\">Click here to download the product bulletin</a> containing detailed physical and transport properties for Therminol 55. </p>
<p><h4>Typical properties</h4></p>
<p>Therminol&reg; 55, Heat Transfer Fluid. Operating Range -25 &deg;C to 290 &deg;C (-15&deg;F to 550 &deg;F) </p>
<table cellspacing=\"0\" cellpadding=\"2\" border=\"1\"><tr>
<td valign=\"top\"><p>Appearance</p></td>
<td valign=\"top\"><p>Clear yellow liquid</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Composition</p></td>
<td valign=\"top\"><p>Synthetic hydrocarbon mixture</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Moisture Content, Maximum </p></td>
<td valign=\"top\"><p>250 ppm</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Flash Point (ASTM D-92) </p></td>
<td valign=\"top\"><p>177 &deg;C (350 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Fire Point (ASTM D-92) </p></td>
<td valign=\"top\"><p>218 &deg;C (425 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Autoignition Temperature (ASTM E-659)</p></td>
<td valign=\"top\"><p>343 &deg;C (650 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Kinematic Viscosity, at 40 &deg;C </p></td>
<td valign=\"top\"><p>19.0 cSt</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Kinematic Viscosity, at 100 &deg;C </p></td>
<td valign=\"top\"><p>3.5 cSt</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Density at 25 &deg;C </p></td>
<td valign=\"top\"><p>868 kg/m3 (7.25 lb/gal)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Specific Gravity (60 &deg;F/60 &deg;F) </p></td>
<td valign=\"top\"><p>0.876</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Coefficient of Thermal Expansion at 200 &deg;C</p></td>
<td valign=\"top\"><p>0.000961/&deg;C (0.000534/&deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Average Molecular Weight </p></td>
<td valign=\"top\"><p>320</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pour Point </p></td>
<td valign=\"top\"><p>-54 &deg;C (-65 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pumpability, at 2000 mm2/s (cSt)</p></td>
<td valign=\"top\"><p>-28 &deg;C (-19 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pumpability, at 300 mm2/s (cSt)</p></td>
<td valign=\"top\"><p>-8 &deg;C (17 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Minimum Temperatures for</p></td>
<td valign=\"top\"><p>&nbsp;</p></td>
</tr>
<tr>
<td colspan=\"2\" valign=\"top\"><blockquote>Fully Developed Turbulent Flow (Re = 10000) </blockquote></td>
</tr>
<tr>
<td valign=\"top\"><p>10 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>67 &deg;C (152 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>20 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>45 &deg;C (114 &deg;F)</p></td>
</tr>
<tr>
<td colspan=\"2\" valign=\"top\"><blockquote>Transition Region Flow (Re = 2000) </blockquote></td>
</tr>
<tr>
<td valign=\"top\"><p>10 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>24 &deg;C (75 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>20 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>11 &deg;C (52 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Boiling Range, 10&percnt; </p></td>
<td valign=\"top\"><p>340 &deg;C (644 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Boiling Range, 90&percnt;</p></td>
<td valign=\"top\"><p>390 &deg;C (734 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Normal Boiling Point </p></td>
<td valign=\"top\"><p>351 &deg;C (664 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Heat of Vaporization at Max. Use Temp 290&deg;C</p></td>
<td valign=\"top\"><p>228 kJ/kg (98.1 Btu/lb)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Optimum Use Range </p></td>
<td valign=\"top\"><p>-25&deg;C to 290&deg;C (-15&deg;F to 550&deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Extended Maximum Use Temperature </p></td>
<td valign=\"top\"><p>315 &deg;C (600 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Maximum Film Temperature </p></td>
<td valign=\"top\"><p>335 &deg;C (635 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Temperature </p></td>
<td valign=\"top\"><p>512 &deg;C (953 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Pressure</p></td>
<td valign=\"top\"><p>13.2 bar (191 psia)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Density </p></td>
<td valign=\"top\"><p>258 kg/m3 (16.1 lb/ft3)</p></td>
</tr>
</table>
<blockquote><br/><br/><b>NOTICE:</b> Although the information and recommendations set forth herein (hereinafter &QUOT;Information&QUOT;) are presented in good faith and believed to be correct as of the date hereof, Solutia makes no representations or warranties as to the completeness or accuracy thereof. Information is supplied upon the condition that the persons receiving same will make their own determination as to its suitability for their purposes prior to use. In no event will Solutia be responsible for damages of any nature whatsoever resulting from the use of or reliance upon Information or the product to which Information refers. Nothing contained herein is to be construed as a recommendation to use any product, process, equipment or formulation in conflict with any patent, and Solutia makes no representation or warranty, express or implied, that the use thereof will not infringe any patent.</blockquote>
<blockquote><br/>NO REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS OR IMPLIED, OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR OF ANY OTHER NATURE ARE MADE HEREUNDER WITH RESPECT TO INFORMATION OR THE PRODUCT TO WHICH INFORMATION REFERS.</blockquote>
<blockquote><br/>Therminol&reg; is a registered trademark of Solutia Inc.</blockquote>
</html>"));
      end Therminol55;

      package Therminol59 "Therminol 59 medium"
        import Modelica.Media;
        import Modelica.SIunits.Conversions;
        extends Media.Incompressible.TableBased(
          mediumName="Therminol 59",
          T_min = Conversions.from_degC(-51),
          T_max = Conversions.from_degC(327),
          TinK = false,
          T0=Conversions.from_degC(-18),
          tableDensity=
            [-51, 1026;
             -46, 1022;
             -40, 1018;
             -29, 1010;
             -18, 1002;
              -7, 994;
               4, 986;
              16, 978;
              27, 970;
              38, 962;
              49, 953;
              60, 945;
              71, 937;
              82, 929;
              93, 921;
             104, 913;
             116, 904;
             127, 896;
             138, 888;
             149, 879;
             160, 871;
             171, 862;
             182, 853;
             193, 845;
             204, 836;
             216, 827;
             227, 818;
             238, 809;
             249, 800;
             260, 790;
             271, 781;
             282, 771;
             289, 765;
             293, 761;
             304, 751;
             316, 740;
             327, 729],
          tableHeatCapacity=
            [-51, 1.45 * 10^3;
             -46, 1.47 * 10^3;
             -40, 1.49 * 10^3;
             -29, 1.52 * 10^3;
             -18, 1.56 * 10^3;
              -7, 1.60 * 10^3;
               4, 1.63 * 10^3;
              16, 1.67 * 10^3;
              27, 1.70 * 10^3;
              38, 1.74 * 10^3;
              49, 1.77 * 10^3;
              60, 1.81 * 10^3;
              71, 1.85 * 10^3;
              82, 1.88 * 10^3;
              93, 1.92 * 10^3;
             104, 1.96 * 10^3;
             116, 1.99 * 10^3;
             127, 2.03 * 10^3;
             138, 2.07 * 10^3;
             149, 2.10 * 10^3;
             160, 2.14 * 10^3;
             171, 2.18 * 10^3;
             182, 2.21 * 10^3;
             193, 2.25 * 10^3;
             204, 2.29 * 10^3;
             216, 2.33 * 10^3;
             227, 2.36 * 10^3;
             238, 2.40 * 10^3;
             249, 2.44 * 10^3;
             260, 2.48 * 10^3;
             271, 2.52 * 10^3;
             282, 2.56 * 10^3;
             289, 2.58 * 10^3;
             293, 2.60 * 10^3;
             304, 2.64 * 10^3;
             316, 2.68 * 10^3;
             327, 2.72 * 10^3],
          tableConductivity=
            [-51, 0.1257;
             -46, 0.1254;
             -40, 0.1251;
             -29, 0.1245;
             -18, 0.1238;
              -7, 0.1231;
               4, 0.1224;
              16, 0.1217;
              27, 0.1209;
              38, 0.1200;
              49, 0.1192;
              60, 0.1183;
              71, 0.1174;
              82, 0.1164;
              93, 0.1154;
             104, 0.1144;
             116, 0.1134;
             127, 0.1123;
             138, 0.1112;
             149, 0.1100;
             160, 0.1088;
             171, 0.1076;
             182, 0.1063;
             193, 0.1051;
             204, 0.1037;
             216, 0.1024;
             227, 0.1010;
             238, 0.0996;
             249, 0.0981;
             260, 0.0966;
             271, 0.0951;
             282, 0.0936;
             289, 0.0926;
             293, 0.0920;
             304, 0.0903;
             316, 0.0887;
             327, 0.0870],
          tableViscosity=
            [-51, 3150 * 10^(-3);
             -46, 1090 * 10^(-3);
             -40, 454 * 10^(-3);
             -29, 118 * 10^(-3);
             -18, 44.9 * 10^(-3);
              -7, 21.8 * 10^(-3);
               4, 12.5 * 10^(-3);
              16, 8.00 * 10^(-3);
              27, 5.56 * 10^(-3);
              38, 4.10 * 10^(-3);
              49, 3.15 * 10^(-3);
              60, 2.51 * 10^(-3);
              71, 2.05 * 10^(-3);
              82, 1.71 * 10^(-3);
              93, 1.45 * 10^(-3);
             104, 1.24 * 10^(-3);
             116, 1.08 * 10^(-3);
             127, 0.947 * 10^(-3);
             138, 0.838 * 10^(-3);
             149, 0.748 * 10^(-3);
             160, 0.671 * 10^(-3);
             171, 0.606 * 10^(-3);
             182, 0.551 * 10^(-3);
             193, 0.503 * 10^(-3);
             204, 0.461 * 10^(-3);
             216, 0.424 * 10^(-3);
             227, 0.392 * 10^(-3);
             238, 0.363 * 10^(-3);
             249, 0.338 * 10^(-3);
             260, 0.315 * 10^(-3);
             271, 0.295 * 10^(-3);
             282, 0.276 * 10^(-3);
             289, 0.266 * 10^(-3);
             293, 0.260 * 10^(-3);
             304, 0.245 * 10^(-3);
             316, 0.231 * 10^(-3);
             327, 0.219 * 10^(-3)],
          tableVaporPressure=
            [ 38, 0.009 * 10^3;
              49, 0.019 * 10^3;
              60, 0.040 * 10^3;
              71, 0.077 * 10^3;
              82, 0.142 * 10^3;
              93, 0.251 * 10^3;
             104, 0.426 * 10^3;
             116, 0.701 * 10^3;
             127, 1.12 * 10^3;
             138, 1.73 * 10^3;
             149, 2.61 * 10^3;
             160, 3.84 * 10^3;
             171, 5.53 * 10^3;
             182, 7.82 * 10^3;
             193, 10.8 * 10^3;
             204, 14.8 * 10^3;
             216, 19.8 * 10^3;
             227, 26.3 * 10^3;
             238, 34.3 * 10^3;
             249, 44.2 * 10^3;
             260, 56.3 * 10^3;
             271, 71.0 * 10^3;
             282, 88.6 * 10^3;
             289, 101 * 10^3;
             293, 109 * 10^3;
             304, 134 * 10^3;
             316, 163 * 10^3;
             327, 196 * 10^3]);
        constant Real[:,2] tableHeatOfVaporization=
            [-51, 389.8;
             -46, 386.5;
             -40, 383.2;
             -29, 376.8;
             -18, 370.5;
              -7, 364.3;
               4, 358.4;
              16, 352.5;
              27, 346.8;
              38, 341.3;
              49, 335.9;
              60, 330.6;
              71, 325.4;
              82, 320.4;
              93, 315.5;
             104, 310.6;
             116, 305.9;
             127, 301.3;
             138, 296.8;
             149, 292.3;
             160, 287.9;
             171, 283.6;
             182, 279.3;
             193, 275.1;
             204, 270.8;
             216, 266.6;
             227, 262.4;
             238, 258.2;
             249, 253.9;
             260, 249.6;
             271, 245.2;
             282, 240.7;
             289, 237.9;
             293, 236.1;
             304, 231.5;
             316, 226.6;
             327, 221.6] "Table for heat of vaporization";
          annotation (Documentation(info="<html>
<p><h4>Therminol 59 Heat Transfer Fluid</h4></p>
<p><br/><b><a href=\"https://team.solutia.com/sites/msds/Therminol%20MSDS%20Documents/197WEN.pdf\">View MSDS</a></b> </p>
<p><b><a href=\"http://www.therminol.com/pages/contact/regform.asp?dir=bulletins&page=default.asp&where=59\">Product Bulletin</a></b> </p>
<p><br/><b>Product description</b></p>
<p>Therminol 59 is a synthetic heat transfer fluid with excellent low temperature pumping characteristics (to -50&deg; F or -45&deg; C) and thermal stability to 600&deg; F (315&deg; C). Performance features of Therminol 59 include:</p>
<p><ul>
<li><b>Heating or Cooling Operation</b> - Therminol 59 has surprisingly low viscosity for a high temperature heat transfer fluid. It is ideally suited for combination heating and cooling applications delivering excellent heat transfer rates even at 70&deg; F (20&deg; C). Batch processes will benefit from the excellent cooling performance Therminol 59 delivers. </li>
<li><b>Superb Low Temperature Pumpability</b> - Low viscosity at exceptionally cold temperatures makes this an ideal fluid for northern climates. In fact, Therminol 59 was originally developed for use on the North Slope in Alaska, a supreme test of low temperature pumpability. </li>
<li><b>Long Life Means Low Cost</b> - Like all the Therminol heat transfer fluids, long fluid life is a given with Therminol 59. Users can expect many years of reliable, trouble-free operation, even when operating continuously at the recommended upper temperature limit of 600&deg; F (315&deg; C). Long life means fewer fluid change-outs and that means low operating costs, which is critical in today&rsquo;s highly competitive business environment. </li>
</ul></p>
<p><h4>Applications</h4></p>
<p>Therminol 59 is used in a wide variety of industries, such as: </p>
<p><ul>
<li>Oil and Gas Processing </li>
<li>Switch Condensers </li>
<li>Pharmaceuticals Manufacturing </li>
<li>Specialty and Batch Chemical Production </li>
<li>Biodiesel </li>
</ul></p>
<p>For more information on Therminol 59 systems and applications <a href=\"mailto:team.therminol@solutia.com\">e-mail</a> us or contact the Therminol Technical Service Hotline at <b>1-800-433-6997</b>. </p>
<p><h4>Detail properties</h4></p>
<p><a href=\"http://www.therminol.com/pages/contact/regform.asp?dir=bulletins&page=default.asp&where=59\">Click here to download the product bulletin</a> containing detailed physical and transport properties for Therminol 59. </p>
<p><h4>Typical properties</h4></p>
<p>Therminol&reg; 59, Heat Transfer Fluid, Operating Range -45 &deg;C to 315 &deg;C (-50 &deg;F to 600 &deg;F) </p>
<table cellspacing=\"0\" cellpadding=\"2\" border=\"1\"><tr>
<td valign=\"top\"><p>Appearance</p></td>
<td valign=\"top\"><p>Light yellow liquid</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Composition</p></td>
<td valign=\"top\"><p>Alkyl substituted aromatic</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Moisture Content, Maximum </p></td>
<td valign=\"top\"><p>200 ppm</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Flash Point (ASTM D-92) </p></td>
<td valign=\"top\"><p>146 &deg;C (295 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Fire Point (ASTM D-92) </p></td>
<td valign=\"top\"><p>154 &deg;C (310 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Autoignition Temperature (ASTM E-659)</p></td>
<td valign=\"top\"><p>404 &deg;C (760 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Kinematic Viscosity, at 40 &deg;C </p></td>
<td valign=\"top\"><p>4.0 cSt</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Kinematic Viscosity, at 100 &deg;C </p></td>
<td valign=\"top\"><p>1.43 cSt</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Density at 25 &deg;C </p></td>
<td valign=\"top\"><p>971 kg/m3 (8.10 lb/gal)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Specific Gravity (60 &deg;F/60 &deg;F) </p></td>
<td valign=\"top\"><p>0.979</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Coefficient of Thermal Expansion at 200 &deg;C</p></td>
<td valign=\"top\"><p>0.000946/&deg;C (0.000525/&deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Average Molecular Weight </p></td>
<td valign=\"top\"><p>207</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pour Point </p></td>
<td valign=\"top\"><p>-68 &deg;C (-90 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pumpability, at 2000 mm2/s (cSt)</p></td>
<td valign=\"top\"><p>-49 &deg;C (-56 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pumpability, at 300 mm2/s (cSt)</p></td>
<td valign=\"top\"><p>-37 &deg;C (-35 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Minimum Temperatures for</p></td>
<td valign=\"top\"><p>&nbsp;</p></td>
</tr>
<tr>
<td colspan=\"2\" valign=\"top\"><blockquote>Fully Developed Turbulent Flow (Re = 10000) </blockquote></td>
</tr>
<tr>
<td valign=\"top\"><p>10 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>17 &deg;C (63 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>20 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>0 &deg;C (32 &deg;F)</p></td>
</tr>
<tr>
<td colspan=\"2\" valign=\"top\"><blockquote>Transition Region Flow (Re = 2000) </blockquote></td>
</tr>
<tr>
<td valign=\"top\"><p>10 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>-16 &deg;C (4 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>20 ft/sec, 1-in tube </p></td>
<td valign=\"top\"><p>-24 &deg;C (-12 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Normal Boiling Point </p></td>
<td valign=\"top\"><p>289 &deg;C (553 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Heat of Vaporization at Max. Use Temp 290&deg;C</p></td>
<td valign=\"top\"><p>227 kJ/kg (97.5 Btu/lb)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Optimum Use Range </p></td>
<td valign=\"top\"><p>-45 &deg;C to 315 &deg;C</p><p>(-50 &deg;F to 600 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Maximum Film Temperature </p></td>
<td valign=\"top\"><p>345 &deg;C (650 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Temperature </p></td>
<td valign=\"top\"><p>514 &deg;C (957 &deg;F)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Pressure</p></td>
<td valign=\"top\"><p>22.3 bar (323 psia)</p></td>
</tr>
<tr>
<td valign=\"top\"><p>Pseudocritical Density </p></td>
<td valign=\"top\"><p>287 kg/m3 (17.9 lb/ft3</p></td>
</tr>
</table>
<blockquote><br/><b>NOTICE:</b> Although the information and recommendations set forth herein (hereinafter &QUOT;Information&QUOT;) are presented in good faith and believed to be correct as of the date hereof, Solutia makes no representations or warranties as to the completeness or accuracy thereof. Information is supplied upon the condition that the persons receiving same will make their own determination as to its suitability for their purposes prior to use. In no event will Solutia be responsible for damages of any nature whatsoever resulting from the use of or reliance upon Information or the product to which Information refers. Nothing contained herein is to be construed as a recommendation to use any product, process, equipment or formulation in conflict with any patent, and Solutia makes no representation or warranty, express or implied, that the use thereof will not infringe any patent.</blockquote>
<blockquote><br/>NO REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS OR IMPLIED, OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR OF ANY OTHER NATURE ARE MADE HEREUNDER WITH RESPECT TO INFORMATION OR THE PRODUCT TO WHICH INFORMATION REFERS.</blockquote>
<blockquote><br/>Therminol&reg; is a registered trademark of Solutia Inc.</blockquote>
</html>"));
      end Therminol59;

      package TherminolVP1
        "Thermal oil -  an eutectic mixture of 73.5% diphenyl oxide (C12H10O) and 26.5% biphenyl (C12H10)"
        extends Modelica.Icons.MaterialPropertiesPackage;

        package TherminolVP1_T "Thermal oil -  an eutectic mixture of 73.5% diphenyl oxide (C12H10O) and 26.5% biphenyl (C12H10)"
          import Cv = Modelica.SIunits.Conversions;

          extends Modelica.Media.Interfaces.PartialMedium(
            final mediumName =   "Therminol VP1",
            final substanceNames = {"C12H10O","C12H10"},
            final ThermoStates = Choices.IndependentVariables.pT,
            final singleState = false,
            final reducedX =  true,
            final fixedX = true,
            reference_X={0.735,0.265});
            //Temperature(min=Cv.from_degC(12), max=Cv.from_degC(400), start=Cv.from_degC(300))

            constant MolarMass MM_C12H10O = 0.1702072;
            constant MolarMass MM_C12H10 =  0.1542078;
            constant Temperature T_start = Cv.from_degC(300);

          redeclare model extends BaseProperties(
            preferredMediumStates=true,
            final standardOrderComponents=true,
            p_bar =  Cv.to_bar(p),
            T_degC = Cv.to_degC(T),
            T(stateSelect=StateSelect.prefer,start = T_start),
            p(stateSelect=StateSelect.prefer)) "Base properties of medium"
            import SolarThermoPro;
            //T_degC(start = T_start - 273.15) = Cv.to_degC(T),
          equation
            // State variables
            state.p = p;
            state.T = T;
            // Remaining variables
            h = 1.377*T_degC^2+1.498*10^3*T_degC-1.8340*10^4;
            d = -0.90797*T_degC + 0.00078116*T_degC^2 -2.367*10^(-6)*T_degC^3 + 1083.25;
            u = h - p/d;
            // R and MM
            MM =
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.MM_X(
              reference_X);
            R  = 8.3144/MM;
          end BaseProperties;

          redeclare record extends ThermodynamicState
          "a selection of variables that uniquely defines the thermodynamic state"
            AbsolutePressure p "pressure";
            Temperature T "temperature";
            annotation (Documentation(info="<html>

</html>"));
          end ThermodynamicState;

          redeclare function extends setState_pTX
          "Returns state record, given pressure and temperatureReturn thermodynamic state as function of p, T and composition X"
          algorithm
            state := ThermodynamicState(p=p,T=T);
          end setState_pTX;

          redeclare function extends setState_dTX
          "Returns state record, given pressure and temperature"
          algorithm
            assert(false, "for incompressible media with d(T) only, state can not be set from density and temperature");
          end setState_dTX;

          redeclare function extends setState_phX
          "Return thermodynamic state as function of p, h and composition X or Xi"
            import SolarThermoPro;
          protected
            Temperature T "temperature";
          algorithm
              T     :=
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.T_h(
              h);
              state := ThermodynamicState(p=p,T=T);
          end setState_phX;

        //   redeclare function extends setState_psX
        //     "Returns state record, given pressure and specific entropy"
        //   protected
        //     Temperature T "temperature";
        //   algorithm
        //       T :=TherminolVP1_Utilities.T_s(s);
        //       state := ThermodynamicState(p=p,T=T);
        //   end setState_psX;

        // redeclare function extends specificInternalEnergy
        //  extends Modelica.Icons.Function;
        // algorithm
        //        u:=TherminolVP1_Utilities.u_pT(state.p, state.T);
        // end specificInternalEnergy;

        redeclare function extends specificEnthalpy
            import SolarThermoPro;
          extends Modelica.Icons.Function;
        algorithm
          h:=SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.h_T(
              state.T);
        end specificEnthalpy;

        // redeclare function extends specificEntropy
        //  extends Modelica.Icons.Function;
        // algorithm
        //        s:=TherminolVP1_Utilities.s_T(state.T);
        // end specificEntropy;

        redeclare function extends density
            import SolarThermoPro;
         extends Modelica.Icons.Function;
        algorithm
               d:=
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.rho_T(
              state.T);
        end density;

          redeclare function extends dynamicViscosity
            import SolarThermoPro;
           extends Modelica.Icons.Function;
          algorithm
               eta :=
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.visc_T(
              state.T);
            annotation (Documentation(info="<html>

</html>"));
          end dynamicViscosity;

          redeclare function extends temperature
           extends Modelica.Icons.Function;
          algorithm
              T := state.T;
            annotation (Documentation(info="<html>

</html>"));
          end temperature;

          redeclare function extends pressure
           extends Modelica.Icons.Function;
          algorithm
              p := state.p;
            annotation (Documentation(info="<html>

</html>"));
          end pressure;

          redeclare function extends thermalConductivity
            import SolarThermoPro;
           extends Modelica.Icons.Function;
          algorithm
                lambda :=
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.cond_T(
              state.T);
            annotation (Documentation(info="<html>

</html>"));
          end thermalConductivity;

          redeclare function extends specificHeatCapacityCp
            import SolarThermoPro;
          algorithm
               cp:=
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_Utilities.cp_T(
              state.T);
            annotation (Documentation(info="<html>

</html>"));
          end specificHeatCapacityCp;

          annotation (Documentation(info="<HTML>
<p>
This package is a <b>template</b> for <b>new medium</b> models. For a new
medium model just make a copy of this package, remove the
\"partial\" keyword from the package and provide
the information that is requested in the comments of the
Modelica source.
</p>
</HTML>"));
        end TherminolVP1_T;

        package TherminolVP1_Utilities
          "Low level and utility computation for molten salts thermodynamic properties"
          extends Modelica.Icons.Package;
          import Cv = Modelica.SIunits.Conversions;
          import SI = Modelica.SIunits;
          import Modelica.Math;
          constant SI.MolarMass MM_C12H10O = 0.1702072;
          constant SI.MolarMass MM_C12H10 =  0.1542078;
          constant Real k = -1.8340*10^4 "Integral constant";

        //   function u_pT "specific internal energy as function of temperature"
        //     extends Modelica.Icons.Function;
        //     input SI.Pressure p "Pressure";
        //     input SI.Temperature T "Temperature";
        //     output SI.SpecificInternalEnergy u "specific internal energy";
        //   protected
        //     parameter SI.SpecificInternalEnergy u0 = 0;//-387687;
        //   algorithm
        //     u := u0 + 0.086*T^2 + 1396.0182*T - (p/(2090 - 0.636*(T-273.15)));
        //   end u_pT;

        //   function s_T "specific entropy as function of temperature"
        //     extends Modelica.Icons.Function;
        //     input SI.Temperature T "Temperature";
        //     output SI.SpecificEntropy s "specific entropy";
        //   protected
        //     parameter SI.SpecificEntropy s0 = 0; //-7878.67;
        //   algorithm
        //     s := s0 + 1396.0182*Modelica.Math.log(T) + 0.172*T;
        //   end s_T;

        //   function T_s "temperature as function of specific entropy"
        //     extends Modelica.Icons.Function;
        //     input SI.SpecificEntropy s "specific entropy";
        //     output SI.Temperature T "Temperature";
        //   algorithm
        //     T :=8116.38*Modelica.Fluid.Dissipation.Utilities.Functions.General.LambertWIter(0.000123208*Modelica.Math.exp(0.000716323*s));
        //   end T_s;

          function dcond_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dconddt
              "partial thermal conductivity derivative with respect to temperature";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            dconddt := -0.0000819477 + T_degC*(-3.84514*10^(-7) + (7.5102*10^(-11) - 2.91896*10^(-14)*T_degC)*T_degC);
          end dcond_dT;

          function dcp_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dcpdt
              "partial specific heat derivative with respect to temperature";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            dcpdt := 2.414 + T_degC * (0.0119182 + (-0.000089637 + 1.76688*10^(-7) * T_degC) * T_degC);
          end dcp_dT;

          function drho_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real drho "partial density derivative with respect to temperature";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            drho := -0.90797 + 2*0.00078116*T_degC -3*2.367*10^(-6)*T_degC^2;
          end drho_dT;

          function dvisc_dT
            extends Modelica.Icons.Function;
            import Modelica.Math;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dviscdt
              "partial viscosity derivative with respect to temperature";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            dviscdt := 1/(114.43 + T_degC)^2 * Math.exp(544.149/(114.43 + T_degC)) * (-0.0448524 + T_degC * (0.0000228783 + T_degC * (-7.96945*10^(-8) + (9.13838*10^(-11) - 5.29647*10^(-13) * T_degC) * T_degC)));
          end dvisc_dT;

          function h_T "specific enthalpy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "Temperature";
            output SI.SpecificEnthalpy h "specific enthalpy";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            //h:= (0.002414*T_degC^2/2 + 5.9591*10^(-6)*T_degC^3/3 - 2.9879*10^(-8)*T_degC^4/4 + 4.4172*10^(-11)*T_degC^5/5 + 1.498*T_degC)*10^3 + k;

            // Koop (2009), SAM (2003)
            //
            h := 1.377*T_degC^2+1.498*10^3*T_degC-1.8340*10^4;
            annotation(derivative=dh_dT);
          end h_T;

          function T_h "temperature as function of specific enthalpy"
            extends Modelica.Icons.Function;
            input SI.SpecificEnthalpy h "specific enthalpy";
            output SI.Temperature T "Temperature";
          protected
            Cv.NonSIunits.Temperature_degC T_degC;
          algorithm
              T_degC := 0.007262164124909223*(-74900+3.1622776601683795*sqrt(5.8625518*10^8+1377*h));
              T := Cv.from_degC(T_degC);
          end T_h;

          function dh_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dhdT "density";
          algorithm
            dhdT := cp_T(T);
          end dh_dT;

          function rho_T "density as function or temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.Density rho "density";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            rho   := -0.90797*T_degC + 0.00078116*T_degC^2 -2.367*10^(-6)*T_degC^3 + 1083.25;
          annotation(derivative=drho_dT);
          end rho_T;

          function cond_T "Thermal conductivity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.ThermalConductivity lambda "Thermal conductivity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            lambda := -8.19477*10^(-5)*T_degC -1.92257*10^(-7)*T_degC^2 + 2.5034*10^(-11)*T_degC^3 - 7.2974*10^(-15)*T_degC^4 + 0.137743;
          annotation(derivative=dcond_dT);
          end cond_T;

          function visc_T "dinamic viscosity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.DynamicViscosity eta "viscosity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            eta := Math.exp(((544.149)/(T_degC+114.43))-2.59578)*10^(-6)*rho_T(T);
          annotation(derivative=dvisc_dT);
          end visc_T;

          function cp_T
            "specific heat capacity at constant pressure as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.SpecificHeatCapacity cp "specific heat capacity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            cp := (0.002414*T_degC + 5.9591*10^(-6)*T_degC^2 - 2.9879*10^(-8)*T_degC^3 + 4.4172*10^(-11)*T_degC^4 + 1.498)*10^3;
          annotation(derivative=dcp_dT);
          end cp_T;

          function MM_X "Molar mass as funtion of mass fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[2] "mass fraction";
            output SI.MolarMass MM "Total molar mass";
          protected
              Real x=toMolarFraction(X);
          algorithm
              MM := x*MM_C12H10O + (1 - x)*MM_C12H10;
          end MM_X;

          function toMolarFraction "Return the molten salts molar fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[2] "Mass fractions";
            output SI.MoleFraction x "Mole";
          algorithm
            x := (X[1]/MM_C12H10O)/(X[1]/MM_C12H10O + (1
               - X[1])/MM_C12H10);
          end toMolarFraction;
        end TherminolVP1_Utilities;
      end TherminolVP1;

      annotation (Icon(graphics={
            Line(
              points = {{-76,-80},{-62,-30},{-32,40},{4,66},{48,66},{73,45},{62,-8},{48,-50},{38,-80}},
              color={64,64,64},
              smooth=Smooth.Bezier),
            Line(
              points={{-40,20},{68,20}},
              color={175,175,175}),
            Line(
              points={{-40,20},{-44,88},{-44,88}},
              color={175,175,175}),
            Line(
              points={{68,20},{86,-58}},
              color={175,175,175}),
            Line(
              points={{-60,-28},{56,-28}},
              color={175,175,175}),
            Line(
              points={{-60,-28},{-74,84},{-74,84}},
              color={175,175,175}),
            Line(
              points={{56,-28},{70,-80}},
              color={175,175,175}),
            Line(
              points={{-76,-80},{38,-80}},
              color={175,175,175}),
            Line(
              points={{-76,-80},{-94,-16},{-94,-16}},
              color={175,175,175})}));
    end ThermalOil;

    package MoltenSalt
                       extends Modelica.Icons.Package;

      package SolarSalt
        "Solar salt -  60% sodium nitrate (NaNO3) 40% potassium nitrate (KNO3)"
        extends Modelica.Icons.MaterialPropertiesPackage;

        package SolarSalt_T
          "Solar salts -  60% sodium nitrate (NaNO3) 40% potassium nitrate (KNO3)"
          import Cv = Modelica.SIunits.Conversions;

          extends Modelica.Media.Interfaces.PartialMedium(
            final mediumName =   "Molten salts",
            final substanceNames = {"NaNO3","KNO3"},
            final ThermoStates = Choices.IndependentVariables.pT,
            final singleState = false,
            final reducedX =  true,
            final fixedX = true,
            reference_X={0.6,0.4},
            Temperature(min=Cv.from_degC(0), max=Cv.from_degC(600), start=Cv.from_degC(300)));

            constant MolarMass MM_NaNO3 = 0.08499478;
            constant MolarMass MM_KNO3 =  0.10110332;
            constant Temperature T_start = Cv.from_degC(300);

          redeclare model extends BaseProperties(
            preferredMediumStates=true,
            final standardOrderComponents=true,
            p_bar =  Cv.to_bar(p),
            T_degC(start = T_start - 273.15) = Cv.to_degC(T),
            T(stateSelect=StateSelect.prefer,start = T_start),
            p(stateSelect=StateSelect.prefer)) "Base properties of medium"
          equation
            // State variables
            state.p = p;
            state.T = T;
            // Remaining variables
            h =SolarSalt_Utilities.h_T(T);
            d = 2090 - 0.636*(T-273.15);
            u = 0.086*T^2 + 1396.0182*T - (p/(2090 - 0.636*(T-273.15)));
            // R and MM
            MM =SolarSalt_Utilities.MM_X(reference_X);
            R  = 8.3144/MM;
          end BaseProperties;

          redeclare record extends ThermodynamicState
          "a selction of variables that uniquely defines the thermodynamic state"
            AbsolutePressure p "pressure";
            Temperature T "temperature";
            annotation (Documentation(info="<html>

</html>"));
          end ThermodynamicState;

          //redeclare function extends setSmoothState
          //  "Return thermodynamic state so that it smoothly approximates: if x > 0 then state_a else state_b"
          //algorithm
          //  state :=ThermodynamicState(p=Modelica.Media.Common.smoothStep(x, state_a.p, state_b.p, x_small),
          //                             T=Modelica.Media.Common.smoothStep(x, state_a.T, state_b.T, x_small));
          //end setSmoothState;

          redeclare function extends setState_pTX
          "Returns state record, given pressure and temperatureReturn thermodynamic state as function of p, T and composition X"
          algorithm
            state := ThermodynamicState(p=p,T=T);
          end setState_pTX;

          redeclare function extends setState_dTX
          "Returns state record, given pressure and temperature"
          algorithm
            assert(false, "for incompressible media with d(T) only, state can not be set from density and temperature");
          end setState_dTX;

          redeclare function extends setState_phX
          "Return thermodynamic state as function of p, h and composition X or Xi"
          protected
            Temperature T "temperature";
          algorithm
              T :=SolarSalt_Utilities.T_h(h);
              state := ThermodynamicState(p=p,T=T);
          end setState_phX;

          redeclare function extends setState_psX
          "Returns state record, given pressure and specific entropy"
          protected
            Temperature T "temperature";
          algorithm
              T :=SolarSalt_Utilities.T_s(s);
              state := ThermodynamicState(p=p,T=T);
          end setState_psX;

        // replaceable record SaturationProperties
        //     "Saturation properties of two phase medium"
        //     extends Modelica.Icons.Record;
        //     AbsolutePressure psat "saturation pressure";
        //     Temperature Tsat "saturation temperature";
        //     MassFraction Xsat[2] "Mass fractions";
        // end SaturationProperties;

        redeclare function extends specificInternalEnergy
         extends Modelica.Icons.Function;
        algorithm
               u:=SolarSalt_Utilities.u_pT(state.p, state.T);
        end specificInternalEnergy;

        redeclare function extends specificEnthalpy
         extends Modelica.Icons.Function;
        algorithm
               h:=SolarSalt_Utilities.h_T(state.T);
        end specificEnthalpy;

        redeclare function extends specificEntropy
         extends Modelica.Icons.Function;
        algorithm
               s:=SolarSalt_Utilities.s_T(state.T);
        end specificEntropy;

        redeclare function extends density
         extends Modelica.Icons.Function;
        algorithm
               d:=SolarSalt_Utilities.rho_T(state.T);
        end density;

          redeclare function extends dynamicViscosity
           extends Modelica.Icons.Function;
          algorithm
               eta :=SolarSalt_Utilities.visc_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end dynamicViscosity;

          redeclare function extends temperature
           extends Modelica.Icons.Function;
          algorithm
              T := state.T;
            annotation (Documentation(info="<html>

</html>"));
          end temperature;

          redeclare function extends pressure
           extends Modelica.Icons.Function;
          algorithm
              p := state.p;
            annotation (Documentation(info="<html>

</html>"));
          end pressure;

          redeclare function extends thermalConductivity
           extends Modelica.Icons.Function;
          algorithm
                lambda :=SolarSalt_Utilities.cond_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end thermalConductivity;

          redeclare function extends specificHeatCapacityCp
          algorithm
               cp:=SolarSalt_Utilities.cp_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end specificHeatCapacityCp;

          annotation (Documentation(info="<HTML>
<p>
This package is a <b>template</b> for <b>new medium</b> models. For a new
medium model just make a copy of this package, remove the
\"partial\" keyword from the package and provide
the information that is requested in the comments of the
Modelica source.
</p>
</HTML>"));
        end SolarSalt_T;

        package SolarSalt_Utilities
          "Low level and utility computation for molten salts thermodynamic properties"
          extends Modelica.Icons.Package;
          import Cv = Modelica.SIunits.Conversions;
          import SI = Modelica.SIunits;
          constant SI.MolarMass MM_NaNO3 =  0.08499478 "NaNO3 molar mass";
          constant SI.MolarMass MM_KNO3 =   0.10110332 "KNO3 molar mass";
          constant Real k = 0 "Integral constant";

        //function dT_dh
        //    "temperature derivative with respect to specific enthalpy"
        //  extends Modelica.Icons.Function;
        //  input SI.SpecificEnthalpy h "specific enthalpy";
        //  output Real dTdh;
        //algorithm
        //  dTdh:= -1/sqrt(2082249-0.344*h);
        //end dT_dh;

          function u_pT "specific internal energy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Pressure p "Pressure";
            input SI.Temperature T "Temperature";
            output SI.SpecificInternalEnergy u "specific internal energy";
          protected
            parameter SI.SpecificInternalEnergy u0 = 0;//-387687;
          algorithm
            u := u0 + 0.086*T^2 + 1396.0182*T - (p/(2090 - 0.636*(T-273.15)));
          end u_pT;

          function s_T "specific entropy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "Temperature";
            output SI.SpecificEntropy s "specific entropy";
          protected
            parameter SI.SpecificEntropy s0 = 0; //-7878.67;
          algorithm
            s := s0 + 1396.0182*Modelica.Math.log(T) + 0.172*T;
          end s_T;

          function h_T "specific enthalpy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "Temperature";
            output SI.SpecificEnthalpy h "specific enthalpy";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
             h:= 0.086*T_degC^2 + 1443*T_degC + k;
          annotation(derivative=dh_dT);
          end h_T;

          function dh_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dhdT "density";
          algorithm
            dhdT := cp_T(T);
          end dh_dT;

          function rho_T "density as function or temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.Density rho "density";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            rho   := 2090 - 0.636*T_degC;
          annotation(derivative=drho_dT);
          end rho_T;

          function drho_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real drho "density";
          algorithm
            drho := -0.636;
          end drho_dT;

          function cond_T "Thermal conductivity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.ThermalConductivity lambda "Thermal conductivity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            lambda := 0.443 + 1.9*10^(-4)*T_degC;
          annotation(derivative=dcond_dT);
          end cond_T;

          function dcond_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dconddt "viscosity";
          algorithm
            dconddt := 0.443;
          end dcond_dT;

          function visc_T "dinamic viscosity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.DynamicViscosity eta "viscosity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            eta := (22.714 - 0.12*T_degC + 2.281*10^(-4)*T_degC^2 - 1.474*10^(-7)*T_degC^3)/1000;//*10^(-3);
          annotation(derivative=dvisc_dT);
          end visc_T;

          function dvisc_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dviscdt "viscosity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            dviscdt := (- 0.12 + 2*2.281*10^(-4)*T_degC - 3*1.474*10^(-7)*T_degC^2)/1000;//*10^(-3);
          end dvisc_dT;

          function cp_T
            "specific heat capacity at constant pressure as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.SpecificHeatCapacity cp "specific heat capacity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            cp := 1443 + 0.172*T_degC;
          annotation(derivative=dcp_dT);
          end cp_T;

          function dcp_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dcpdt "viscosity";
          algorithm
            dcpdt := 1443;
          end dcp_dT;

          function T_s "temperature as function of specific entropy"
            extends Modelica.Icons.Function;
            input SI.SpecificEntropy s "specific entropy";
            output SI.Temperature T "Temperature";
          algorithm
            T :=8116.38*Modelica.Fluid.Dissipation.Utilities.Functions.General.LambertWIter(0.000123208*Modelica.Math.exp(0.000716323*s));
          end T_s;

          function T_h "temperature as function of specific enthalpy"
            extends Modelica.Icons.Function;
            input SI.SpecificEnthalpy h "specific enthalpy";
            output SI.Temperature T "Temperature";
            //
            // h = 0.086*T^2 + 1443*T + k
            //
          protected
            Cv.NonSIunits.Temperature_degC T_degC;
          algorithm
              T_degC := (-1443+sqrt(1443^2 - 4*0.086*(k-h)))/(2*0.086);
              T := Cv.from_degC(T_degC);
          end T_h;

          function MM_X "Molar mass as funtion of mass fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[2] "mass fraction";
            output SI.MolarMass MM "Total molar mass";
          protected
              Real x=toMolarFraction(X);
          algorithm
              MM := x*MM_NaNO3 + (1 - x)*MM_KNO3;
          end MM_X;

          function toMolarFraction "Return the molten salts molar fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[2] "Mass fractions";
            output SI.MoleFraction x "Mole";
          algorithm
            x := (X[1]/MM_NaNO3)/(X[1]/MM_NaNO3 + (1
               - X[1])/MM_KNO3);
          end toMolarFraction;
        end SolarSalt_Utilities;
      end SolarSalt;

      package Hitec
        extends Modelica.Icons.MaterialPropertiesPackage;

        package Hitec_T "Hitec -  7% sodium nitrate (NaNO3), 53% potassium nitrate (KNO3) and 40% sodium nitrite (NaNO2)"
          import Cv = Modelica.SIunits.Conversions;

          extends Modelica.Media.Interfaces.PartialMedium(
            final mediumName =   "Hitec",
            final substanceNames = {"NaNO3","KNO3","NaNO2"},
            final ThermoStates = Choices.IndependentVariables.pT,
            final singleState = false,
            final reducedX =  true,
            final fixedX = true,
            reference_X={0.07,0.53,0.4},
            Temperature(min=Cv.from_degC(-10), max=Cv.from_degC(600), start=Cv.from_degC(292)));

            constant MolarMass MM_NaNO3 = 0.08499478;
            constant MolarMass MM_KNO3 =  0.10110332;
            constant MolarMass MM_NaNO2 = 0.06899535;
            constant Temperature T_start = Cv.from_degC(292);

          redeclare model extends BaseProperties(
            preferredMediumStates=true,
            final standardOrderComponents=true,
            p_bar =  Cv.to_bar(p),
            T_degC(start = T_start - 273.15) = Cv.to_degC(T),
            T(stateSelect=StateSelect.prefer,start = T_start),
            p(stateSelect=StateSelect.prefer)) "Base properties of medium"
          equation
            // State variables
            state.p = p;
            state.T = T;
            // Remaining variables
            h = 1663.43 * (T-273.15) - 0.2230 * (T-273.15)^2; //-0.25*(T-273.15)^2 + 1672.6*(T-273.15); //1560*(T-273.15); //-0.5*(T-273.15)^2 + 1560*(T-273.15); //Hitec_Utilities.h_T(T);
            d = 2080 - 0.733*(T-273.15);
            u = h - p/d; //0.086*T^2 + 1396.0182*T - (p/(2090 - 0.636*(T-273.15)));
            // R and MM
            MM =Hitec_Utilities.MM_X(reference_X);
            R  = 8.3144/MM;
          end BaseProperties;

          redeclare record extends ThermodynamicState
          "a selction of variables that uniquely defines the thermodynamic state"
            AbsolutePressure p "pressure";
            Temperature T "temperature";
            annotation (Documentation(info="<html>

</html>"));
          end ThermodynamicState;

          redeclare function extends setState_pTX
          "Returns state record, given pressure and temperatureReturn thermodynamic state as function of p, T and composition X"
          algorithm
            state := ThermodynamicState(p=p,T=T);
          end setState_pTX;

          redeclare function extends setState_dTX
          "Returns state record, given pressure and temperature"
          algorithm
            assert(false, "for incompressible media with d(T) only, state can not be set from density and temperature");
          end setState_dTX;

          redeclare function extends setState_phX
          "Return thermodynamic state as function of p, h and composition X or Xi"
          protected
            Temperature T "temperature";
          algorithm
              T :=Hitec_Utilities.T_h(h);
              state := ThermodynamicState(p=p,T=T);
          end setState_phX;

        //replaceable record SaturationProperties
        //    "Saturation properties of two phase medium"
        //    extends Modelica.Icons.Record;
        //    AbsolutePressure psat "saturation pressure";
        //    Temperature Tsat "saturation temperature";
        //    MassFraction Xsat[2] "Mass fractions";
        //end SaturationProperties;

        redeclare function extends specificInternalEnergy
         extends Modelica.Icons.Function;
        algorithm
               u:=Hitec_Utilities.u_pT(state.p, state.T);
        end specificInternalEnergy;

        redeclare function extends specificEnthalpy
         extends Modelica.Icons.Function;
        algorithm
               h:=Hitec_Utilities.h_T(state.T);
        end specificEnthalpy;

        redeclare function extends density
         extends Modelica.Icons.Function;
        algorithm
               d:=Hitec_Utilities.rho_T(state.T);
        annotation(der_density);
        end density;

          redeclare function extends dynamicViscosity
           extends Modelica.Icons.Function;
          algorithm
               eta :=Hitec_Utilities.visc_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end dynamicViscosity;

          redeclare function extends temperature
           extends Modelica.Icons.Function;
          algorithm
              T := state.T;
            annotation (Documentation(info="<html>

</html>"));
          end temperature;

          redeclare function extends pressure
           extends Modelica.Icons.Function;
          algorithm
              p := state.p;
            annotation (Documentation(info="<html>

</html>"));
          end pressure;

          redeclare function extends thermalConductivity
           extends Modelica.Icons.Function;
          algorithm
                lambda :=Hitec_Utilities.cond_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end thermalConductivity;

          redeclare function extends specificHeatCapacityCp
          algorithm
               cp:=Hitec_Utilities.cp_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end specificHeatCapacityCp;

          function der_density
           extends Modelica.Icons.Function;
          algorithm
                 d:=Hitec_Utilities.drho_dT(state.T,0);
          end der_density;
          annotation (Documentation(info="<HTML>
<p>
This package is a <b>template</b> for <b>new medium</b> models. For a new
medium model just make a copy of this package, remove the
\"partial\" keyword from the package and provide
the information that is requested in the comments of the
Modelica source.
</p>
</HTML>"));
        end Hitec_T;

        package Hitec_Utilities "Low level and utility computation for molten salts thermodynamic properties"
          extends Modelica.Icons.Package;
          import Cv = Modelica.SIunits.Conversions;
          import SI = Modelica.SIunits;
          constant SI.MolarMass MM_NaNO3 =  0.08499478 "NaNO3 molar mass";
          constant SI.MolarMass MM_KNO3 =   0.10110332 "KNO3 molar mass";
          constant SI.MolarMass MM_NaNO2 = 0.06899535;
          constant Real k = 0 "Integral constant";

        // function s_T "specific entropy as function of temperature"
        //   extends Modelica.Icons.Function;
        //   input SI.Temperature T "Temperature";
        //   output SI.SpecificEntropy s "specific entropy";
        //   protected
        //   parameter SI.SpecificEntropy s0 = 0; //-7878.67;
        // algorithm
        //   s := s0 + 1396.0182*Modelica.Math.log(T) + 0.172*T;
        // end s_T;

        // function dvisc_dT
        //   extends Modelica.Icons.Function;
        //   input SI.Temperature T "temperature";
        //   input Real dT "temperature derivative";
        //   output Real dviscdt "viscosity";
        //   protected
        //   Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
        // algorithm
        //   dviscdt := (- 0.12 + 2*2.281*10^(-4)*T_degC - 3*1.474*10^(-7)*T_degC^2)/1000;//*10^(-3);
        // end dvisc_dT;

        // function T_s "temperature as function of specific entropy"
        //   extends Modelica.Icons.Function;
        //   input SI.SpecificEntropy s "specific entropy";
        //   output SI.Temperature T "Temperature";
        // algorithm
        //   T :=8116.38*Modelica.Fluid.Dissipation.Utilities.Functions.General.LambertWIter(0.000123208*Modelica.Math.exp(0.000716323*s));
        // end T_s;

          function u_pT "specific internal energy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Pressure p "Pressure";
            input SI.Temperature T "Temperature";
            output SI.SpecificInternalEnergy u "specific internal energy";
          protected
            parameter SI.SpecificInternalEnergy u0 = 0;//-387687;
          algorithm
            u := h_T(T) - p/rho_T(T);
          end u_pT;

          function h_T "specific enthalpy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "Temperature";
            output SI.SpecificEnthalpy h "specific enthalpy";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
             //h:= 1560*T_degC + k;    //-0.5*T_degC^2 + 1560*T_degC + k;

             //h := 1000*(-0.0005*T_degC^2 + 1.6726*T_degC) + k;

             //h := -0.25*T_degC^2 + 1672.6*T_degC + k;

             h :=  1663.43 * T_degC - 0.2230 * T_degC^2 + k;
          annotation(derivative=dh_dT);
          end h_T;

          function dh_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dhdT "density";
          algorithm
            dhdT := cp_T(T);
          annotation(derivative=ddh_ddT);
          end dh_dT;

          function ddh_ddT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            input Real dT2 "temperature";
            input Real ddT2 "temperature derivative";
            output Real ddhddT "density";
          algorithm
           ddhddT := dcp_dT(T,dT);
          end ddh_ddT;

          function rho_T "density as function or temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.Density rho "density";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            //rho   := 2080 - 0.733*T_degC;
            rho   := 1973.5 - 0.417*T_degC;
            annotation(derivative=drho_dT);
          end rho_T;

          function drho_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real drho "density";
          algorithm
            //drho := -0.733;
            drho := -0.417;
            annotation(derivative=ddrho_ddT);
          end drho_dT;

          function ddrho_ddT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            input Real dT2 "temperature";
            input Real ddT2 "temperature derivative";
            output Real ddrho "density";
          algorithm
            ddrho := 0;
          end ddrho_ddT;

          function cond_T "Thermal conductivity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.ThermalConductivity lambda "Thermal conductivity";
          algorithm
            lambda := 0.78 - 1.25*10^(-3)*T + 1.6*10^(-6)*T^2;
          annotation(derivative=dcond_dT);
          end cond_T;

          function dcond_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dconddt "viscosity";
          algorithm
            dconddt := - 1.25*10^(-3) + 2*1.6*10^(-6)*T;
          end dcond_dT;

          function visc_T "dinamic viscosity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.DynamicViscosity eta "viscosity";
          protected
               Real b,ebp,ebm;
          algorithm
            b   := 5.9*(T - 9.638)/990.362;
            ebp := Modelica.Math.exp(b);
            ebm := Modelica.Math.exp(-b);
            eta := ((ebp+ebm)/(ebp-ebm))-0.999;
          annotation(derivative=dvisc_dT);
          end visc_T;

          function dvisc_dT "dinamic viscosity derivative as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output SI.Pressure deta "viscosity";
          protected
               Real b,et;
          algorithm
            b    := 0.0119148*T;
            et   := Modelica.Math.exp(b);
            deta := -((0.0267295*et)/((-1.12169+et)^2));
          end dvisc_dT;

          function cp_T
            "specific heat capacity at constant pressure as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.SpecificHeatCapacity cp "specific heat capacity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            //cp := 1000*(-0.0005*T_degC + 1.6726);

            //cp := 1560 - T_degC*1;

            cp :=  1663.43 - 0.4460 * T_degC;
          annotation(derivative=dcp_dT);
          end cp_T;

          function dcp_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dcpdt "viscosity";
          algorithm
            //dcpdt := 1000*(-0.0005); //1560;
            dcpdt := -0.4460;
          end dcp_dT;

          function T_h "temperature as function of specific enthalpy"
            extends Modelica.Icons.Function;
            input SI.SpecificEnthalpy h "specific enthalpy";
            output SI.Temperature T "Temperature";
            //
            // h:= -0.5*T_degC^2 + 1560*T_degC + k;
            //

            //
            // h := -0.25*T_degC^2 + 1672.6*T_degC + k;
            //

            //
            // h :=  1663.43 * T_degc - 0.2230 * T_degC^2 + k;
            //
          protected
            Cv.NonSIunits.Temperature_degC T_degC;
          algorithm
            //T_degC := -1560 + sqrt(1560^2 +2*(k-h));

            //T_degC := -3.309*10^(-24)*h^2 + 6.41*10^(-4)*h + 1.364*10^(-12);

            //T_degC := (-1672.6 + sqrt(1672.6^2 -4*(-0.25)*(k-h)))/(2*(-0.25));

            T_degC := (-1663.43 + sqrt(1663.43^2 -4*(-0.2230)*(k-h)))/(2*(-0.2230));

            T      := Cv.from_degC(T_degC);

          end T_h;

          function MM_X "Molar mass as funtion of mass fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[3] "mass fraction";
            output SI.MolarMass MM "Total molar mass";
          protected
              Real x[3]=toMolarFraction(X);
          algorithm
              MM := x[1]*MM_NaNO3 + x[2]*MM_KNO3 + x[3]*MM_NaNO2;
          end MM_X;

          function toMolarFraction "Return the molten salts molar fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[3] "Mass fractions";
            output SI.MoleFraction x[3] "Mole";
          protected
            Real total = (X[1]/MM_NaNO3 + (X[2])/MM_KNO3 + (X[3])/MM_NaNO2);
          algorithm
            x[1] := (X[1]/MM_NaNO3)/total;
            x[2] := (X[2]/MM_KNO3)/total;
            x[3] := (X[3]/MM_NaNO2)/total;
          end toMolarFraction;
        end Hitec_Utilities;
      end Hitec;

      package HitecXL
        extends Modelica.Icons.MaterialPropertiesPackage;

        package HitecXL_T
          "Hitec XL -  42% calcium nitrate (Ca(NO3)2 15% sodium nitrate (NaNO3) and 43% potassium nitrate (KNO3)"
          import Cv = Modelica.SIunits.Conversions;

          extends Modelica.Media.Interfaces.PartialMedium(
            final mediumName =   "Hitec XL",
            final substanceNames = {"Ca(NO2)2","NaNO3","KNO3"},
            final ThermoStates = Choices.IndependentVariables.pT,
            final singleState = false,
            final reducedX =  true,
            final fixedX = true,
            reference_X={0.42,0.15,0.43},
            Temperature(min=Cv.from_degC(-10), max=Cv.from_degC(600), start=Cv.from_degC(150)));

            constant MolarMass MM_Ca_NO3_2 = 1.59408;
            constant MolarMass MM_NaNO3 =    0.08499478;
            constant MolarMass MM_KNO3 =     0.10110332;
            constant Temperature T_start =   Cv.from_degC(150);

          redeclare model extends BaseProperties(
            preferredMediumStates=true,
            final standardOrderComponents=true,
            p_bar =  Cv.to_bar(p),
            T_degC(start = T_start - 273.15) = Cv.to_degC(T),
            T(stateSelect=StateSelect.prefer,start = T_start),
            p(stateSelect=StateSelect.prefer)) "Base properties of medium"
          equation
            // State variables
            state.p = p;
            state.T = T;
            // Remaining variables
            h = -3.79667*10^(-5)*(T-273.15)^3 -1.312*10^(-1)*(T-273.15)^2 + 1.536*10^3*(T-273.15);
            d = -8.266*10^(-1)*(T-273.15) + 2.240*10^3;
            u = h - p/d;
            // R and MM
            MM =HitecXL_Utilities.MM_X(reference_X);
            R  = 8.3144/MM;
          end BaseProperties;

          redeclare record extends ThermodynamicState
          "a selction of variables that uniquely defines the thermodynamic state"
            AbsolutePressure p "pressure";
            Temperature T "temperature";
            annotation (Documentation(info="<html>

</html>"));
          end ThermodynamicState;

          redeclare function extends setState_pTX
          "Returns state record, given pressure and temperatureReturn thermodynamic state as function of p, T and composition X"
          algorithm
            state := ThermodynamicState(p=p,T=T);
          end setState_pTX;

          redeclare function extends setState_dTX
          "Returns state record, given pressure and temperature"
          algorithm
            assert(false, "for incompressible media with d(T) only, state can not be set from density and temperature");
          end setState_dTX;

          redeclare function extends setState_phX
          "Return thermodynamic state as function of p, h and composition X or Xi"
          protected
            Temperature T "temperature";
          algorithm
              T :=HitecXL_Utilities.T_h(h);
              state := ThermodynamicState(p=p,T=T);
          end setState_phX;

        //replaceable record SaturationProperties
        //    "Saturation properties of two phase medium"
        //    extends Modelica.Icons.Record;
        //    AbsolutePressure psat "saturation pressure";
        //    Temperature Tsat "saturation temperature";
        //    MassFraction Xsat[2] "Mass fractions";
        //end SaturationProperties;

        redeclare function extends specificInternalEnergy
         extends Modelica.Icons.Function;
        algorithm
               u:=HitecXL_Utilities.u_pT(state.p, state.T);
        end specificInternalEnergy;

        redeclare function extends specificEnthalpy
         extends Modelica.Icons.Function;
        algorithm
               h:=HitecXL_Utilities.h_T(state.T);
        end specificEnthalpy;

        redeclare function extends density
         extends Modelica.Icons.Function;
        algorithm
               d:=HitecXL_Utilities.rho_T(state.T);
        annotation(der_density);
        end density;

          redeclare function extends dynamicViscosity
           extends Modelica.Icons.Function;
          algorithm
               eta :=HitecXL_Utilities.visc_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end dynamicViscosity;

          redeclare function extends temperature
           extends Modelica.Icons.Function;
          algorithm
              T := state.T;
            annotation (Documentation(info="<html>

</html>"));
          end temperature;

          redeclare function extends pressure
           extends Modelica.Icons.Function;
          algorithm
              p := state.p;
            annotation (Documentation(info="<html>

</html>"));
          end pressure;

          redeclare function extends thermalConductivity
           extends Modelica.Icons.Function;
          algorithm
                lambda :=HitecXL_Utilities.cond_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end thermalConductivity;

          redeclare function extends specificHeatCapacityCp
          algorithm
               cp:=HitecXL_Utilities.cp_T(state.T);
            annotation (Documentation(info="<html>

</html>"));
          end specificHeatCapacityCp;

          function der_density
           extends Modelica.Icons.Function;
          algorithm
                 d:=HitecXL_Utilities.drho_dT(state.T, 0);
          end der_density;
          annotation (Documentation(info="<HTML>
<p>
This package is a <b>template</b> for <b>new medium</b> models. For a new
medium model just make a copy of this package, remove the
\"partial\" keyword from the package and provide
the information that is requested in the comments of the
Modelica source.
</p>
</HTML>"));
        end HitecXL_T;

        package HitecXL_Utilities
          "Low level and utility computation for molten salts thermodynamic properties"
          extends Modelica.Icons.Package;
          import Cv = Modelica.SIunits.Conversions;
          import SI = Modelica.SIunits;
            constant SI.MolarMass MM_Ca_NO3_2 = 1.59408;
            constant SI.MolarMass MM_NaNO3 =    0.08499478;
            constant SI.MolarMass MM_KNO3 =     0.10110332;
          constant Real k = 0 "Integral constant";

        // function s_T "specific entropy as function of temperature"
        //   extends Modelica.Icons.Function;
        //   input SI.Temperature T "Temperature";
        //   output SI.SpecificEntropy s "specific entropy";
        //   protected
        //   parameter SI.SpecificEntropy s0 = 0; //-7878.67;
        // algorithm
        //   s := s0 + 1396.0182*Modelica.Math.log(T) + 0.172*T;
        // end s_T;

        // function dvisc_dT
        //   extends Modelica.Icons.Function;
        //   input SI.Temperature T "temperature";
        //   input Real dT "temperature derivative";
        //   output Real dviscdt "viscosity";
        //   protected
        //   Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
        // algorithm
        //   dviscdt := (- 0.12 + 2*2.281*10^(-4)*T_degC - 3*1.474*10^(-7)*T_degC^2)/1000;//*10^(-3);
        // end dvisc_dT;

        // function T_s "temperature as function of specific entropy"
        //   extends Modelica.Icons.Function;
        //   input SI.SpecificEntropy s "specific entropy";
        //   output SI.Temperature T "Temperature";
        // algorithm
        //   T :=8116.38*Modelica.Fluid.Dissipation.Utilities.Functions.General.LambertWIter(0.000123208*Modelica.Math.exp(0.000716323*s));
        // end T_s;

          function u_pT "specific internal energy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Pressure p "Pressure";
            input SI.Temperature T "Temperature";
            output SI.SpecificInternalEnergy u "specific internal energy";
          protected
            parameter SI.SpecificInternalEnergy u0 = 0;//-387687;
          algorithm
            u := h_T(T) - p/rho_T(T);
          end u_pT;

          function h_T "specific enthalpy as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "Temperature";
            output SI.SpecificEnthalpy h "specific enthalpy";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
             h:= -3.79667*10^(-5)*T_degC^3 -1.312*10^(-1)*T_degC^2 + 1.536*10^3*T_degC;
          annotation(derivative=dh_dT);
          end h_T;

          function dh_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dhdT "density";
          algorithm
            dhdT := cp_T(T);
          annotation(derivative=ddh_ddT);
          end dh_dT;

          function ddh_ddT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            input Real dT2 "temperature";
            input Real ddT2 "temperature derivative";
            output Real ddhddT "density";
          algorithm
           ddhddT := dcp_dT(T,dT);
          end ddh_ddT;

          function rho_T "density as function or temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.Density rho "density";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            rho   := -8.266*10^(-1)*T_degC + 2.240*10^3;
            annotation(derivative=drho_dT);
          end rho_T;

          function drho_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real drho "density";
          algorithm
            drho := -8.266*10^(-1);
            annotation(derivative=ddrho_ddT);
          end drho_dT;

          function ddrho_ddT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            input Real dT2 "temperature";
            input Real ddT2 "temperature derivative";
            output Real ddrho "density";
          algorithm
            ddrho := 0;
          end ddrho_ddT;

          function cond_T "Thermal conductivity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.ThermalConductivity lambda "Thermal conductivity";
          algorithm
            lambda := 0.78 - 1.25*10^(-3)*T + 1.6*10^(-6)*T^2;
          annotation(derivative=dcond_dT);
          end cond_T;

          function dcond_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dconddt "viscosity";
          algorithm
            dconddt := - 1.25*10^(-3) + 2*1.6*10^(-6)*T;
          end dcond_dT;

          function visc_T "dinamic viscosity as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.DynamicViscosity eta "viscosity";
          protected
               Real b,ebp,ebm;
          algorithm
            b   := 5.9*(T - 9.638)/990.362;
            ebp := Modelica.Math.exp(b);
            ebm := Modelica.Math.exp(-b);
            eta := ((ebp+ebm)/(ebp-ebm))-0.999;
          annotation(derivative=dvisc_dT);
          end visc_T;

          function dvisc_dT "dinamic viscosity derivative as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output SI.Pressure deta "viscosity";
          protected
               Real b,et;
          algorithm
            b    := 0.0119148*T;
            et   := Modelica.Math.exp(b);
            deta := -((0.0267295*et)/((-1.12169+et)^2));
          end dvisc_dT;

          function cp_T
            "specific heat capacity at constant pressure as function of temperature"
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            output SI.SpecificHeatCapacity cp "specific heat capacity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            cp := -1.139*10^(-4) * T_degC^2 - 2.624*10^(-1)*T_degC + 1.536*10^3;
          annotation(derivative=dcp_dT);
          end cp_T;

          function dcp_dT
            extends Modelica.Icons.Function;
            input SI.Temperature T "temperature";
            input Real dT "temperature derivative";
            output Real dcpdt "viscosity";
          protected
            Cv.NonSIunits.Temperature_degC T_degC = Cv.to_degC(T);
          algorithm
            dcpdt := -2*1.139*10^(-4) * T_degC - 2.624*10^(-1);
          end dcp_dT;

          function T_h "temperature as function of specific enthalpy"
            extends Modelica.Icons.Function;
            input SI.SpecificEnthalpy h "specific enthalpy";
            output SI.Temperature T "Temperature";
            //
            // h:= -0.5*T_degC^2 + 1560*T_degC + k;
            //
          protected
            Cv.NonSIunits.Temperature_degC T_degC;
          algorithm
            //T_degC := -1560 + sqrt(1560^2 +2*(k-h));
            //T := Cv.from_degC(T_degC);
            T_degC := 5.111*10^(-11)*h^2 + 6.466*10^(-4)*h + 2.*151*10^(-1);
            T      := Cv.from_degC(T_degC);
          end T_h;

          function MM_X "Molar mass as funtion of mass fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[3] "mass fraction";
            output SI.MolarMass MM "Total molar mass";
          protected
              Real x[3]=toMolarFraction(X);
          algorithm
              MM := x[1]*MM_Ca_NO3_2 + x[2]*MM_KNO3 + x[3]*MM_NaNO3;
          end MM_X;

          function toMolarFraction "Return the molten salts molar fraction"
            extends Modelica.Icons.Function;
            input SI.MassFraction X[3] "Mass fractions";
            output SI.MoleFraction x[3] "Mole";
          protected
            Real total = (X[1]/MM_Ca_NO3_2 + (X[2])/MM_KNO3 + (X[3])/MM_NaNO3);
          algorithm
            x[1] := (X[1]/MM_Ca_NO3_2)/total;
            x[2] := (X[2]/MM_KNO3)/total;
            x[3] := (X[3]/MM_NaNO3)/total;
          end toMolarFraction;
        end HitecXL_Utilities;
      end HitecXL;
      annotation (Icon(graphics={
            Line(
              points = {{-76,-80},{-62,-30},{-32,40},{4,66},{48,66},{73,45},{62,-8},{48,-50},{38,-80}},
              color={64,64,64},
              smooth=Smooth.Bezier),
            Line(
              points={{-40,20},{68,20}},
              color={175,175,175}),
            Line(
              points={{-40,20},{-44,88},{-44,88}},
              color={175,175,175}),
            Line(
              points={{68,20},{86,-58}},
              color={175,175,175}),
            Line(
              points={{-60,-28},{56,-28}},
              color={175,175,175}),
            Line(
              points={{-60,-28},{-74,84},{-74,84}},
              color={175,175,175}),
            Line(
              points={{56,-28},{70,-80}},
              color={175,175,175}),
            Line(
              points={{-76,-80},{38,-80}},
              color={175,175,175}),
            Line(
              points={{-76,-80},{-94,-16},{-94,-16}},
              color={175,175,175})}));
    end MoltenSalt;
    annotation (Icon(graphics={
          Line(
            points = {{-76,-80},{-62,-30},{-32,40},{4,66},{48,66},{73,45},{62,-8},{48,-50},{38,-80}},
            color={64,64,64},
            smooth=Smooth.Bezier),
          Line(
            points={{-40,20},{68,20}},
            color={175,175,175}),
          Line(
            points={{-40,20},{-44,88},{-44,88}},
            color={175,175,175}),
          Line(
            points={{68,20},{86,-58}},
            color={175,175,175}),
          Line(
            points={{-60,-28},{56,-28}},
            color={175,175,175}),
          Line(
            points={{-60,-28},{-74,84},{-74,84}},
            color={175,175,175}),
          Line(
            points={{56,-28},{70,-80}},
            color={175,175,175}),
          Line(
            points={{-76,-80},{38,-80}},
            color={175,175,175}),
          Line(
            points={{-76,-80},{-94,-16},{-94,-16}},
            color={175,175,175})}));
  end Media;

  package Test
               extends Modelica.Icons.ExamplesPackage;

    package ThermalOil
                       extends Modelica.Icons.ExamplesPackage;

      model Therminol55
                   extends Modelica.Icons.Example;
                   extends SolarThermoPro.Base.TPBaseMediaIncompressible(
                            final TdegC_start = Tstart, final TdegC_stop = Tstop,
                            final TdegC_min = -29, final TdegC_max = 316,
                            final nullValue = 0,
                            redeclare package medium =
              SolarThermoPro.Media.ThermalOil.Therminol55);

      import Modelica.SIunits.Conversions;

      parameter Conversions.NonSIunits.Temperature_degC Tstart = -29 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC Tstop =  316 "Final temperature (ºC)";

      end Therminol55;

      model Therminol59
                   extends Modelica.Icons.Example;
                   extends SolarThermoPro.Base.TPBaseMediaIncompressible(
                            final TdegC_start = Tstart, final TdegC_stop = Tstop,
                            final TdegC_min = -50, final TdegC_max = 320,
                            final nullValue = 0,
                            redeclare package medium =
              SolarThermoPro.Media.ThermalOil.Therminol59);

      import Modelica.SIunits.Conversions;

      parameter Conversions.NonSIunits.Temperature_degC Tstart = -50 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC Tstop = 320 "Final temperature (ºC)";

      end Therminol59;

      model TherminolVP1
                   extends Modelica.Icons.Example;
                   extends SolarThermoPro.Base.TPBaseMediaIncompressible(
                            final TdegC_start = Tstart, final TdegC_stop = Tstop,
                            final TdegC_min = 12, final TdegC_max = 425,
                            final nullValue = 0,
                            redeclare package medium =
              SolarThermoPro.Media.ThermalOil.TherminolVP1.TherminolVP1_T);

      import Modelica.SIunits.Conversions;

      parameter Conversions.NonSIunits.Temperature_degC Tstart = 12 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC Tstop = 425 "Final temperature (ºC)";

      end TherminolVP1;
    end ThermalOil;

    package MoltenSalt
                       extends Modelica.Icons.ExamplesPackage;

      model SolarSalt
                   extends Modelica.Icons.Example;
                   extends SolarThermoPro.Base.TPBaseMediaIncompressible(
                            final TdegC_start = Tstart, final TdegC_stop = Tstop,
                            final TdegC_min = 220, final TdegC_max = 600,
                            final nullValue = 0,
                            redeclare package medium =
              SolarThermoPro.Media.MoltenSalt.SolarSalt.SolarSalt_T);

      import Modelica.SIunits.Conversions;

      parameter Conversions.NonSIunits.Temperature_degC Tstart = 220 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC Tstop = 600 "Final temperature (ºC)";

      end SolarSalt;

      model HitecReview
        extends Modelica.Icons.Example;

        // Import
        import Modelica.SIunits;
        import Modelica.Media;
        import Modelica.SIunits.Conversions;

        // Outputs
        output Conversions.NonSIunits.Temperature_degC TdegC;
        output SIunits.Density rho_Kirst,rho_Janz,rho_Yang,rho_Wu,rho_Boerema,rho_Serrano,rho_Siegel;
        output SIunits.DynamicViscosity v_Kirst,v_Janz,v_Gaune,v_Yang,v_Chen,v_Siegel,v_Wu,v_Boerema;
        output SIunits.ThermalConductivity k_Janz,k_Santini,k_Yang,k_Wu,k_Siegel;
        output SIunits.SpecificHeatCapacity cp_Janz,cp_Boerema,cp_Yang,cp_Wu,cp_Serrano,cp_Proposed;

        // Parameters
        parameter Conversions.NonSIunits.Temperature_degC Tstart = 142 "Initial temperature (ºC)";
        parameter Conversions.NonSIunits.Temperature_degC Tstop =  535 "Final temperature (ºC)";

      protected
        SIunits.Temperature T = Conversions.from_degC(TdegC);
        Real b = 5.9*(T-9.638)/990.362;

      equation

        // Temperature
        TdegC = Tstart + (Tstop - Tstart)*time;

        // Density
        rho_Kirst   = if TdegC>=142 and TdegC<=535 then 2082.6 - 0.7288 * TdegC else 0;
        rho_Janz    = if T    >=470 and T    <=870 then 2293.6 - 0.7497 * T else 0;
        rho_Yang    = if TdegC>=149 and TdegC<=538 then 1938 - 0.732 * (TdegC-200) else 0;
        rho_Siegel =  if TdegC>=142 and TdegC<=538 then 2084 - 0.740 * TdegC else 0;
        rho_Wu      = if TdegC>=220 and TdegC<=500 then 2083.5 - 0.748 * TdegC else 0;
        rho_Boerema = if TdegC>=142 and TdegC<=535 then 2080 - 0.733 * TdegC else 0;
        rho_Serrano = if T    >=448 and T    <=773 then 2279.799 - 0.7324 * (T) else 0;

        // Dynamic viscosity
        v_Kirst   = if TdegC>=142 and TdegC<=535 then 67.57 * 10^(-6) * exp(2247.11/T) else 0;
        v_Janz    = if T    >=420 and T    <=710 then 10^(-3)*(473.7 - 2.297*T + 3.731*10^(-3)*T^2 - 2.019*10^(-6)*T^3) else 0;
        v_Gaune   = if TdegC>=200 and TdegC<=500 then 10^(-3)*0.5631*exp(-146.9794*(T)^(-1)+574265.2*(T)^(-2)) else 0;
        v_Yang    = if TdegC>=149 and TdegC<=538 then exp(-4.343-2.0143*(log(TdegC)-5.011)) else 0;
        v_Chen    = if TdegC>=250 and TdegC<=500 then 10^(-3)*0.75484*exp(6418/(8.314*T)) else 0;
        v_Siegel  = if TdegC>=142 and TdegC<=538 then 10^(-3)*(TdegC)^(-2.104)*10^5.7374 else 0;
        v_Wu      = if TdegC>=220 and TdegC<=500 then 0.2149*exp(-(TdegC)/57.05) + 0.0017 else 0;
        v_Boerema = if TdegC>=142 and TdegC<=535 then ((exp(b)+exp(-b))/(exp(b)-exp(-b)))-0.999 else 0;

        // Thermal conductivity
        k_Janz    = if T    >=430 and T    <=670 then 0.6836 - 4.6024*10^(-4)*(T) else 0;
        k_Santini = if TdegC>=150 and TdegC<=500 then 0.78 - 1.25*10^(-3)*T+1.6*10^(-6)*T^2 else 0;
        k_Yang    = if TdegC>=149 and TdegC<=538 then 0.421 - 6.53*10^(-4)*(TdegC-260.0) else 0;
        k_Wu      = if TdegC>=300 and TdegC<=500 then 0.586 - 6.4*10^(-4)*TdegC else 0;
        k_Siegel  = if TdegC>=142 and TdegC<=538 then 0.411 + 4.36*10^(-4)*TdegC - 1.54*10^(-6)*TdegC^2 else 0;

        // Specific heat capacity
        cp_Janz     = if T    >=426 and T    <=776 then 5806 - 10.833*T + 7.2413*10^(-3)*T^2 else 0;
        cp_Boerema  = if TdegC>=142 and TdegC<=535 then 1560 - TdegC else 0;
        cp_Yang     = if TdegC>=149 and TdegC<=538 then 1561.7 else 0;
        cp_Wu       = if TdegC>=220 and TdegC<=500 then 1424 else 0;
        cp_Serrano  = if T    >=448 and T    <=773 then 1560 else 0;
        cp_Proposed = if TdegC>=142 and TdegC<=535 then 1663.43 - 0.4460 * TdegC else 0;

        annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
              coordinateSystem(preserveAspectRatio=false)));
      end HitecReview;

      model HitecXL
                   extends Modelica.Icons.Example;
                   extends SolarThermoPro.Base.TPBaseMediaIncompressible(
                            final TdegC_start = Tstart, final TdegC_stop = Tstop,
                            final TdegC_min = 133, final TdegC_max = 500,
                            final nullValue = 0,
                            redeclare package medium =
              SolarThermoPro.Media.MoltenSalt.HitecXL.HitecXL_T);

      import Modelica.SIunits.Conversions;

      parameter Conversions.NonSIunits.Temperature_degC Tstart = 133 "Initial temperature (ºC)";
      parameter Conversions.NonSIunits.Temperature_degC Tstop = 500 "Final temperature (ºC)";

      end HitecXL;
    end MoltenSalt;
  end Test;
  annotation (uses(Modelica(version="3.2.2")), Icon(graphics={
        Ellipse(
          lineColor={255,130,0},
          fillColor={255,250,90},
          pattern=LinePattern.None,
          fillPattern=FillPattern.Sphere,
          extent={{-44,-48},{50,44}})}));
end SolarThermoPro;
