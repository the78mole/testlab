--------------------------------------------------------------------------------
-- Copyright (c) 1995-2008 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 10.1.03
--  \   \         Application : xaw2vhdl
--  /   /         Filename : clk_synth_18_8.vhd
-- /___/   /\     Timestamp : 03/14/2009 13:55:02
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: xaw2vhdl-st C:\testlab_lrs\trunk\HW_DEV\IPCORES\\clk_synth_18_8.xaw C:\testlab_lrs\trunk\HW_DEV\IPCORES\\clk_synth_18_8
--Design Name: clk_synth_18_8
--Device: xc3s500e-4pq208
--
-- Module clk_synth_18_8
-- Generated by Xilinx Architecture Wizard
-- Adjusted by Daniel Glaser
-- Written for synthesis tool: XST
-- Period Jitter (unit interval) for block DCM_SP_INST1 = 0.05 UI
-- Period Jitter (Peak-to-Peak) for block DCM_SP_INST1 = 2.45 ns
-- Period Jitter (unit interval) for block DCM_SP_INST2 = 0.04 UI
-- Period Jitter (Peak-to-Peak) for block DCM_SP_INST2 = 4.80 ns

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library UNISIM;
use UNISIM.Vcomponents.all;

entity clk_synth_18_8_adj is
  port (U1_CLKIN_IN        : in  std_logic;
        U2_CLKIN_IN        : in  std_logic;
        RESET_IN           : in  std_logic;
        U1_CLKIN_IBUFG_OUT : out std_logic;
        U2_CLKIN_IBUFG_OUT : out std_logic;
        LOCKED_OUT         : out std_logic;
        CLK_8_OUT          : out std_logic;
        CLK_18_OUT         : out std_logic);
end clk_synth_18_8_adj;

architecture BEHAVIORAL of clk_synth_18_8_adj is
  signal GND_BIT        : std_logic;
  signal U1_CLKIN_IBUFG : std_logic;
  signal U1_CLK0_BUF    : std_logic;
  signal U1_CLKFB_IN    : std_logic;
  signal U1_CLKFX_BUF   : std_logic;
  signal U2_CLKIN_IBUFG : std_logic;
  signal U2_CLK0_BUF    : std_logic;
  signal U2_CLKFB_IN    : std_logic;
  signal U2_CLKFX_BUF   : std_logic;
  signal U1_LOCKED      : std_logic;
  signal U2_LOCKED      : std_logic;
  signal U1_USED        : std_logic;
  signal U2_USED        : std_logic;
  signal MUX_18_U1      : std_logic;
  signal MUX_8_U1       : std_logic;
  
begin
  GND_BIT            <= '0';
  U1_CLKIN_IBUFG_OUT <= U1_CLKIN_IBUFG;
  U2_CLKIN_IBUFG_OUT <= U2_CLKIN_IBUFG;

  MUX_18_U1 <= not U2_LOCKED;
  MUX_8_U1  <= not U2_LOCKED;

  LOCKED_OUT <= U1_LOCKED or U2_LOCKED;

  BUFGMUX_18 : BUFGMUX
    port map (I0 => U1_CLKFX_BUF,
              I1 => U2_CLK0_BUF,
              S  => MUX_18_U1,
              O  => CLK_18_OUT);

  BUFGMUX_8 : BUFGMUX
    port map (I0 => U1_CLK0_BUF,
              I1 => U2_CLKFX_BUF,
              S  => MUX_8_U1,
              O  => CLK_8_OUT);

  DCM_SP_INST1 : DCM_SP
    generic map(CLK_FEEDBACK          => "1X",
                CLKDV_DIVIDE          => 2.0,
                CLKFX_DIVIDE          => 10,
                CLKFX_MULTIPLY        => 23,
                CLKIN_DIVIDE_BY_2     => false,
                CLKIN_PERIOD          => 125.000,
                CLKOUT_PHASE_SHIFT    => "NONE",
                DESKEW_ADJUST         => "SYSTEM_SYNCHRONOUS",
                DFS_FREQUENCY_MODE    => "LOW",
                DLL_FREQUENCY_MODE    => "LOW",
                DUTY_CYCLE_CORRECTION => true,
                FACTORY_JF            => x"C080",
                PHASE_SHIFT           => 0,
                STARTUP_WAIT          => false)
    port map (CLKFB    => U1_CLKFB_IN,
              CLKIN    => U1_CLKIN_IBUFG,
              DSSEN    => GND_BIT,
              PSCLK    => GND_BIT,
              PSEN     => GND_BIT,
              PSINCDEC => GND_BIT,
              RST      => RESET_IN,
              CLKDV    => open,
              CLKFX    => U1_CLKFX_BUF,
              CLKFX180 => open,
              CLK0     => U1_CLK0_BUF,
              CLK2X    => open,
              CLK2X180 => open,
              CLK90    => open,
              CLK180   => open,
              CLK270   => open,
              LOCKED   => U1_LOCKED,
              PSDONE   => open,
              STATUS   => open);

  DCM_SP_INST2 : DCM_SP
    generic map(CLK_FEEDBACK          => "1X",
                CLKDV_DIVIDE          => 2.0,
                CLKFX_DIVIDE          => 23,
                CLKFX_MULTIPLY        => 10,
                CLKIN_DIVIDE_BY_2     => false,
                CLKIN_PERIOD          => 54.348,
                CLKOUT_PHASE_SHIFT    => "NONE",
                DESKEW_ADJUST         => "SYSTEM_SYNCHRONOUS",
                DFS_FREQUENCY_MODE    => "LOW",
                DLL_FREQUENCY_MODE    => "LOW",
                DUTY_CYCLE_CORRECTION => true,
                FACTORY_JF            => x"C080",
                PHASE_SHIFT           => 0,
                STARTUP_WAIT          => false)
    port map (CLKFB    => U2_CLKFB_IN,
              CLKIN    => U2_CLKIN_IBUFG,
              DSSEN    => GND_BIT,
              PSCLK    => GND_BIT,
              PSEN     => GND_BIT,
              PSINCDEC => GND_BIT,
              RST      => RESET_IN,
              CLKDV    => open,
              CLKFX    => U2_CLKFX_BUF,
              CLKFX180 => open,
              CLK0     => U2_CLK0_BUF,
              CLK2X    => open,
              CLK2X180 => open,
              CLK90    => open,
              CLK180   => open,
              CLK270   => open,
              LOCKED   => U2_LOCKED,
              PSDONE   => open,
              STATUS   => open);

  U1_CLKIN_IBUFG_INST : IBUFG
    port map (I => U1_CLKIN_IN,
              O => U1_CLKIN_IBUFG);

  U2_CLKIN_IBUFG_INST : IBUFG
    port map (I => U2_CLKIN_IN,
              O => U2_CLKIN_IBUFG);

  U1_CLK0_BUFG_INST : BUFG
    port map (I => U1_CLK0_BUF,
              O => U1_CLKFB_IN);

  U2_CLK0_BUFG_INST : BUFG
    port map (I => U2_CLK0_BUF,
              O => U2_CLKFB_IN);

end BEHAVIORAL;


