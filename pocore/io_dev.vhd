library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
--use ieee.std_logic_signed.all;


entity io_dev is


	port(
		CLK		:	in	std_logic;
		CPU_WR	:	in	std_logic;
		CPU_RD	:	in	std_logic;
		CPU_OUT	:	in	std_logic_vector(31 downto 0);
		CPU_IN	:	out	std_logic_vector(31 downto 0);
		NYET	:	out std_logic;
		RS_RX	:	in	std_logic;
		RS_TX	:	out	std_logic
	);


end io_dev;

architecture RTL of io_dev is
	component u232c is

  generic (wtime: std_logic_vector(15 downto 0) := x"1ADB");
  Port ( clk  : in  std_logic;
		 send_data : in  std_logic_vector (7 downto 0);
		 send_go   : in  std_logic;
		 send_busy : out std_logic;
		 tx   : out std_logic;
		 recv_data : out std_logic_vector (7 downto 0);
		 recv_ready : out std_logic;
		 rx   : in std_logic
		);

	end component;

	subtype buf_rec_t is std_logic_vector(7 downto 0);

-- sendbuf min 2048 (contest.sld & baudrate 115200)

	type sendbuf_t is array (0 to 2047) of buf_rec_t;
	signal sendbuf : sendbuf_t;
	signal send_head	: std_logic_vector(10 downto 0) := (others=>'0');
	signal send_tail	: std_logic_vector(10 downto 0) := (others=>'0');

	signal send_empty : std_logic := '0';

	signal recv_head	: std_logic_vector(10 downto 0) := (others=>'0');
	signal recv_tail	: std_logic_vector(10 downto 0) := (others=>'0');
	type recvbuf_t is array (0 to 2047) of buf_rec_t;

	signal recvbuf : recvbuf_t;
	--signal recvbuf : recvbuf_t := (
	--);
	signal u232c_o : std_logic_vector(7 downto 0);
	signal u232c_i : std_logic_vector(7 downto 0);

	signal send_go : std_logic;
	signal send_busy : std_logic := '0';

	signal recv_ready : std_logic;

	
begin
	
	rs232c: u232c generic map (wtime=>x"0243")
	port map (clk, u232c_o, send_go, send_busy, RS_TX, 
				   u232c_i, recv_ready, RS_RX);

------------------------------------------------------------------------
---- send
------------------------------------------------------------------------
	send_empty <= '1' when send_head=send_tail else '0';
	send: process(CLK)
	begin
		if rising_edge(CLK) then
			sendbuf(conv_integer(send_tail)) <= CPU_OUT(7 downto 0);
			if CPU_WR='1' then
				send_tail <= send_tail + 1;
			end if;

			u232c_o <= sendbuf(conv_integer(send_head));
			if send_empty='0' and send_busy='0' and send_go='0' then
				send_go<='1';
				send_head <= send_head+1;
			else
				send_go<='0';
			end if;

		end if;
	end process;

------------------------------------------------------------------------
---- recv
------------------------------------------------------------------------
	NYET <= '1' when recv_head=recv_tail else '0';
	receive : process(CLK)
	begin
		if rising_edge(CLK) then
			CPU_IN <= x"000000"&recvbuf(conv_integer(recv_head));
			if CPU_RD='1' then
				recv_head <= recv_head+1;
			end if;

			recvbuf(conv_integer(recv_tail))<=u232c_i;
			if recv_ready='1' then
				recv_tail <= recv_tail+1;
			end if;
		end if;
	end process;


end RTL;



