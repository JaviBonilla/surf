within ;
package HMT_Chapter1
  model Example_1_1 "Heating a copper ball"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter SIunits.Density              rho =             8950 "Density (kg/m3)";
    parameter SIunits.Diameter             D =               0.1  "Diameter (m)";
    parameter SIunits.SpecificHeatCapacity cp(min=0) =       395  "Heat cap. (J/(kg.ºC))";
    parameter NonSIunits.Temperature_degC  T1(min=-273.15) = 100  "Initial temp. (ºC)";
    parameter NonSIunits.Temperature_degC  T2(min=-273.15) = 150  "Final temp. (ºC)";
    parameter SIunits.Time                 dT(min=1e-9) =    1800 "Time interval (s)";

    output SIunits.Area         A    "Area (m2)";
    output SIunits.Volume       V    "Volume (m3)";
    output SIunits.Mass         m    "Mass (kg)";
    output SIunits.Heat         Q    "Heat transferred (J)";
    output SIunits.HeatFlowRate Qavg "Heat flow rate (W)";
    output SIunits.HeatFlux     qavg "Heat flux (W/m2)";

  equation
    A    = Constants.pi*D^2;
    V    = A*D/6;
    m    = rho*V;
    Q    = m*cp*(T2-T1);
    Q    = Qavg*dT;
    Qavg = qavg*A;

  end Example_1_1;

  model Example_1_2 "Cooling of Stainless Steel Sheets"
    import Modelica.SIunits;

    parameter SIunits.Velocity             v(min=0) =     0.01  "Velocity (m/s)";
    parameter SIunits.Length               t(min=0) =     0.005 "Thickness (m)";
    parameter SIunits.Length               w(min=0) =     2     "Lenght (m)";
    parameter SIunits.Temperature          T_in =         500   "Inlet temperature (K)";
    parameter SIunits.Temperature          T_out =        300   "Outlet temperature (K)";
    parameter SIunits.SpecificHeatCapacity cp(min=0) =    515   "Heat capacity (J/(kg K))";
    parameter SIunits.Density              rho =          7900  "Density (kg/m^3)";

    output SIunits.MassFlowRate m_dot  "Mass flow rate";
    output SIunits.HeatFlowRate Q_loss "Heat flow rate";

  equation
    m_dot  = rho*v*w*t;
    Q_loss = m_dot * cp * (T_in - T_out);

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_2;

  model Example_1_3 "Heat Loss from Heating Ducts in a Basement"
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions;
    import Modelica.SIunits.Conversions.NonSIunits;

    constant Real R = 287 "Gas constant ((Pa m^3)/(Kg K))";

    parameter SIunits.Length               len(min=0) =         5           "Lenght (m)";
    parameter SIunits.Length               L1(min=0) =          0.20        "Side 1 lenght (m)";
    parameter SIunits.Length               L2(min=0) =          0.25        "Side 2 lenght (m)";
    parameter SIunits.Pressure             p(min=0) =           100*10^3    "Pressure (Pa)";
    parameter NonSIunits.Temperature_degC  T_in(min=-273.15) =  60          "Inlet temperature (ºC)";
    parameter NonSIunits.Temperature_degC  T_out(min=-273.15) = 54          "Outlet temperature (ºC)";
    parameter SIunits.Velocity             v(min=0) =           5           "Velocity (m/s)";
    parameter SIunits.SpecificHeatCapacity cp(min=0) =          1007        "Specific heat capacity (J/(Kg K))";
    parameter SIunits.Efficiency           e(min=0,max=1) =     0.80        "Efficiency (%)";
    parameter Real                         cost_gas(min=0) =    1.60        "Cost of natural gas ($/therm)";
    parameter SIunits.Energy               therm(min=0) =       105500*10^3 "1 therm in joules (J)";

    output SIunits.Area                  A     "Area (m^2)";
    output SIunits.Density               rho   "Density (kg/m^3)";
    output SIunits.MassFlowRate          m_dot "Mass flow rate (kg/s)";
    output SIunits.TemperatureDifference dT    "Temperature difference (K)";
    output SIunits.HeatFlowRate          Q     "Heat flow rate (W)";
    output Real                          cost  "Cost ($/h)";

  equation
     rho   = p/(R*Conversions.from_degC(T_in));
     A     = L1*L2;
     m_dot = rho*v*A;
     dT    = T_in - T_out;
     Q     = m_dot *cp * dT;
     cost  = (Q*3600*cost_gas)/(e*therm);

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_3;

  model Example_1_4 "Electric Heating of a House at High Elevation"
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions;
    import Modelica.SIunits.Conversions.NonSIunits;

    constant Real R = 287 "Gas constant ((Pa m^3)/(Kg K))";

    parameter SIunits.Area                                   A(min=0) =        200   "Area (m^2)";
    parameter SIunits.Length                                 L(min=0) =        3     "Lenght (m)";
    parameter SIunits.Pressure                               P_atm(min=0) =    84600 "Pressure (Pa)";
    parameter NonSIunits.Temperature_degC                    T1(min=-273.15) = 10    "Initial temperature (ºC)";
    parameter NonSIunits.Temperature_degC                    T2(min=-273.15) = 20    "Final temperature (ºC)";
    parameter SIunits.SpecificHeatCapacityAtConstantPressure cp(min=0) =       1007  "Spec. heat cap. cte. pressure (J/(Kg K))";
    parameter Real                                           cost_e(min=0) =   0.075 "Cost of electricity ($/kWh)";

    output SIunits.SpecificHeatCapacityAtConstantVolume cv      "Spec. heat cap. cte. volume (J/(Kg K))";
    output SIunits.Volume                               V       "Volume (m^3)";
    output SIunits.TemperatureDifference                dT      "Temperature difference (ºC)";
    output SIunits.Mass                                 m       "Mass (Kg)";
    output SIunits.Energy                               E_cte_v "Energy (J)";
    output SIunits.Energy                               E_cte_p "Energy (J)";
    output Real                                         cost_v  "Cost ($)";
    output Real                                         cost_p  "Cost ($)";

  equation
    V       = A*L;
    m       = P_atm*V/(R*Conversions.from_degC(T1));
    cv      = cp - R;
    dT      = T2-T1;
    E_cte_v = m*cv*dT;
    E_cte_p = m*cp*dT;
    cost_v  = E_cte_v*cost_e/(3600*10^3);
    cost_p  = E_cte_p*cost_e/(3600*10^3);

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_4;

  model Example_1_5 "The Cost of Heat Loss through a Roof"
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions.NonSIunits;

    constant Real R = 287 "Gas constant ((Pa m^3)/(Kg K))";

    parameter SIunits.Length              L1(min=0) =          6    "Lenght (m)";
    parameter SIunits.Length              L2(min=0) =          8    "Lenght (m)";
    parameter SIunits.Length              tck(min=0) =         0.25 "Tickness (m)";
    parameter SIunits.ThermalConductivity k(min=0) =           0.8  "Thermal conductivity (W/(m K))";
    parameter NonSIunits.Temperature_degC T_in(min=-273.15) =  15   "Inner temperature (ºC)";
    parameter NonSIunits.Temperature_degC T_out(min=-273.15) = 4    "Outer temperature (ºC)";
    parameter SIunits.Time                dt(min=0) =          10   "Time interval (h)";
    parameter Real                        cost_e(min=0) =      0.08  "Cost of electricity ($/kWh)";

    output SIunits.Area                  A     "Area (m^2)";
    output SIunits.TemperatureDifference dT    "Temperature difference (ºC)";
    output SIunits.HeatFlowRate          Q_dot "Heat flow rate (W)";
    output Real                          Q     "Heat loss (Wh)";
    output Real                          cost  "Cost ($)";

  equation
    A     = L1*L2;
    dT    = T_in - T_out;
    Q_dot = k*A*dT/tck;
    Q     = Q_dot*dt;
    cost  = Q*cost_e/1000;

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_5;

  model Example_1_6 "Measuring the Thermal Conductivity of a Material"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter SIunits.Diameter            D =         0.05 "Diameter (m)";
    parameter SIunits.Length              L(min=0) =  0.03 "Lenght (m)";
    parameter SIunits.Voltage             V(min=0) =  110  "Voltage (V)";
    parameter SIunits.Current             I(min=0) =  0.4  "Current (A)";
    parameter NonSIunits.Temperature_degC dT =        15   "Temperature difference (ºC)";

    output SIunits.Area                A  "Area (m^2)";
    output SIunits.Power               We "Power (W)";
    output SIunits.HeatFlowRate        Q  "Heat flow rate (W)";
    output SIunits.ThermalConductivity k "Thermal conductivity (W/(m K))";

  equation
    A  = 1/4*Constants.pi*D^2;
    We = V*I;
    Q  = 1/2*We;
    Q  = k*A*dT/L;

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_6;

  model Example_1_7 "Conversion between SI and English Units"
    import Modelica.SIunits;

    constant Real WtoBtu = 3.41214 "W to Btu";
    constant Real mtoft =  3.2808  "m to ft";
    constant Real CtoF =   1.8     "C to F";

    parameter SIunits.ThermalConductivity k_SI(min=0) = 0.72 "Thermal conductivity (W/(m ºC))";
    output Real k_IMP "Thermal conductivity (Btu/(h ft ºF))";

  equation
    k_IMP = k_SI*WtoBtu/(mtoft*CtoF);

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_7;

  model Example_1_8 "Measuring Convection Heat Transfer Coefficient"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter SIunits.Length              l(min=0) =            0.2  "Lenght (m)";
    parameter SIunits.Diameter            D =                   0.03 "Diameter (m)";
    parameter NonSIunits.Temperature_degC T_surr(min=-273.15) = 15   "Surrounding temperature (ºC)";
    parameter NonSIunits.Temperature_degC T_s(min=-273.15) =    152  "Surface temperature (ºC)";
    parameter SIunits.Current             I(min=0) =            1.5  "Current (A)";
    parameter SIunits.Voltage             V(min=0) =            60   "Voltage (V)";

    output SIunits.Area                      A_s     "Surface area (m^2)";
    output SIunits.HeatFlowRate              Q_conv  "Convective heat flow rate (W/m^2)";
    output SIunits.CoefficientOfHeatTransfer h       "Convective heat transfer coefficient (W/(m^2 K))";

  equation
    A_s    = Constants.pi*l*D;
    Q_conv = V*I;
    Q_conv = h*A_s*(T_s-T_surr);
    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_8;

  model Example_1_9 " Radiation Effect on Thermal Comfort"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter Real                        epsilon(min=0,max=1) =       0.95 "Emissivity (-)";
    parameter SIunits.Area                A_s(min=0) =                 1.4  "Area (m^2)";
    parameter NonSIunits.Temperature_degC T_surr(min=-273.15) =        15   "Surrounding temperature (ºC)";
    parameter NonSIunits.Temperature_degC T_s(min=-273.15) =           30   "Surface temperature (ºC)";
    parameter NonSIunits.Temperature_degC T_wall_summer(min=-273.15) = 25   "Wall temperature in summer (ºC)";
    parameter NonSIunits.Temperature_degC T_wall_winter(min=-273.15) = 10   "Wall temperature in summer (ºC)";


    output SIunits.HeatFlowRate Q_rad_summer "Radiation heat losses in summer (W)";
    output SIunits.HeatFlowRate Q_rad_winter "Radiation heat losses in winter (W)";

    SIunits.Temperature T_s_K =           Conversions.from_degC(T_s);
    SIunits.Temperature T_wall_winter_K = Conversions.from_degC(T_wall_winter);
    SIunits.Temperature T_wall_summer_K = Conversions.from_degC(T_wall_summer);

  equation
    Q_rad_summer = epsilon*Constants.sigma*A_s*(T_s_K^4-T_wall_winter_K^4);
    Q_rad_winter = epsilon*Constants.sigma*A_s*(T_s_K^4-T_wall_summer_K^4);
    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_9;

  model Example_1_10 " Heat Loss from a Person"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter Real                              epsilon(min=0,max=1) = 0.95 "Emissivity (-)";
    parameter SIunits.CoefficientOfHeatTransfer h(min=0) =             6    "Heat transfer coefficient (W/(m^2 K))";
    parameter SIunits.Area                      A_s(min=0) =           1.6  "Area (m^2)";
    parameter NonSIunits.Temperature_degC       T_surr(min=-273.15) =  20   "Surrounding temperature (ºC)";
    parameter NonSIunits.Temperature_degC       T_s(min=-273.15) =     29   "Surface temperature (ºC)";

    output SIunits.HeatFlowRate Q_rad  "Radiation heat losses (W)";
    output SIunits.HeatFlowRate Q_conv "Convection heat losses (W)";
    output SIunits.HeatFlowRate Q_cond "Conduction heat losses (W)";
    output SIunits.HeatFlowRate Q_t    "Total heat losses (W)";

    SIunits.Temperature T_s_K =    Conversions.from_degC(T_s);
    SIunits.Temperature T_surr_K = Conversions.from_degC(T_surr);

  equation
    Q_t    = Q_rad + Q_conv + Q_cond;
    Q_rad  = epsilon*Constants.sigma*A_s*(T_s_K^4-T_surr_K^4);
    Q_conv = h*A_s*(T_s-T_surr);
    Q_cond = 0;

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_10;

  model Example_1_11 " Heat Transfer between Two Isothermal Plates"
    import Modelica.Constants;
    import Modelica.SIunits;

    parameter SIunits.Temperature         T1 =                   300     "Plate 1 temperature (K)";
    parameter SIunits.Temperature         T2 =                   200     "Plate 2 temperature (K)";
    parameter SIunits.Length              L(min=0) =             0.01    "Lenght (m)";
    parameter SIunits.Area                A(min=0) =             1       "Area (m^2)";
    parameter Real                        epsilon(min=0,max=1) = 1       "Emissivity (-)";
    parameter SIunits.ThermalConductivity k_air(min=0) =         0.0219  "Air thermal conductivity (W/(m K))";
    parameter SIunits.ThermalConductivity k_ure(min=0) =         0.026   "Urethane insulation thermal conductivity (W/(m K))";
    parameter SIunits.ThermalConductivity k_sup(min=0) =         0.00002 "Superinsulation thermal conductivity (W/(m K))";

    output SIunits.HeatFlowRate Q_cond_a  "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_cond_c  "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_cond_d  "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_rad     "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_a       "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_b       "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_c       "Heat flow rate (W)";
    output SIunits.HeatFlowRate Q_d       "Heat flow rate (W)";

    Real Q_cond_par;

  equation
    Q_cond_par = A*(T1-T2)/L;
    Q_cond_a   = k_air*Q_cond_par;
    Q_cond_c   = k_ure*Q_cond_par;
    Q_cond_d   = k_sup*Q_cond_par;
    Q_rad      = epsilon*Constants.sigma*A*(T1^4-T2^4);
    Q_a        = Q_cond_a + Q_rad;
    Q_b        = Q_rad;
    Q_c        = Q_cond_c;
    Q_d        = Q_cond_d;

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_11;

  model Example_1_13 "Heating of a Plate by Solar Energy"
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter SIunits.HeatFlux                  Irr(min=0) =           700  "Solar radiation (W/m^2)";
    parameter Real                              alpha(min=0,max=1) =   0.6  "Absorptivity (-)";
    parameter SIunits.CoefficientOfHeatTransfer h(min=0) =             50   "Heat transfer coefficient (W/(m^2 K))";
    parameter NonSIunits.Temperature_degC       T_surr(min=-273.15) =  25   "Surrounding temperature (ºC)";

    output SIunits.EnergyFlowRate      E_gain "Energy gained (J/m^2)";
    output SIunits.EnergyFlowRate      E_lost "Energy lost (J/m^2)";
    output NonSIunits.Temperature_degC T_s    "Plate surface temperature (ºC)";

  equation
    E_gain = alpha*Irr;
    E_lost = h*(T_s-T_surr);
    E_gain = E_lost;

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_13;

  model Example_1_14 "Fire Hazard Prevention of Oil Leakage on Hot Engine Surface"
    import Modelica.Constants;
    import Modelica.SIunits;
    import Modelica.SIunits.Conversions;
    import Modelica.SIunits.Conversions.NonSIunits;

    parameter SIunits.HeatFlux                  q_0(min=0) =           5000 "Heat flux (W/m^2)";
    parameter Real                              epsilon(min=0,max=1) = 0.3  "Emissivity (-)";
    parameter SIunits.CoefficientOfHeatTransfer h(min=0) =             15   "Heat transfer coefficient (W/(m^2 K))";
    parameter NonSIunits.Temperature_degC       T_surr(min=-273.15) =  40   "Surrounding temperature (ºC)";

    output NonSIunits.Temperature_degC T_0      "Surface temperature (ºC)";
    output NonSIunits.Temperature_degC T_0_K    "Surface temperature (ºC)";
    output SIunits.Temperature         T_surr_K "Surrounding temperature (K)";

  equation
    T_0_K    = Conversions.from_degC(T_0);
    T_surr_K = Conversions.from_degC(T_surr);
    q_0      = h*(T_0 - T_surr) + epsilon*Constants.sigma*(T_0_K^4 - T_surr_K^4);

    annotation (Icon(coordinateSystem(preserveAspectRatio=false)), Diagram(
          coordinateSystem(preserveAspectRatio=false)));
  end Example_1_14;
end HMT_Chapter1;
