#include <ZubrRPC.hpp>
#include <cassert>
#include <iostream>

using ZP = Zubr::RPC::Procedures;

int main(int argc, char** argv) {
  assert(argc == 2);

  size_t nRecv = std::stoul(argv[1]);

  Zubr::IO::TTYConfig conf;

  conf.Baudrate = 3000000;
  conf.ParityBit = false;
  conf.Stopbits = Zubr::IO::TTYConfig::StopbitsCount::One;
  conf.Timeout = 1;

  conf.Port = "/dev/ttyACM0";

  auto io = Zubr::IO(conf);

  Zubr::RPC::Call<ZP::SetZubrParam>(io);
  Zubr::RPC::Call<ZP::GetZubrParam>(io);
  Zubr::RPC::Call<ZP::UnitParam>(io);
  Zubr::RPC::Call<ZP::SetPosition>(io);
  Zubr::RPC::Call<ZP::GetPosition>(io);
  Zubr::RPC::Call<ZP::GetVelocity>(io);
  Zubr::RPC::Call<ZP::GetIMU>(io);
  Zubr::RPC::Call<ZP::StartSlot>(io);

  /*
  Zubr::CsMessageOut msgOut;
  msgOut.hostBeginQuery(RB_CB_RL_IMU_GET);
  msgOut.hostEnd();
  
  io.Send(msgOut);
  */
  try {
    //auto msgIn = io.Recv(nRecv);

    //auto resp = Zubr::RPC::Call<ZP::SetZubrParam>(io);

    // std::cout << "Sio: " << +resp.Sio() << std::endl;
    // std::cout << "Oper: " << +resp.Oper() << std::endl;
    // std::cout << "Id: " << +resp.Id() << std::endl;
    // std::cout << "Index: " << +resp.Index() << std::endl;
    // std::cout << "Value: " << +resp.Value() << std::endl;

  } catch (std::exception& e) {
    std::cout << "Got exception: " << e.what() << std::endl;
  }

  std::cout << std::hex;
  for (int i = 0; i < nRecv; ++i) std::cout << +io.RxBuf[i] << " ";
  std::cout << std::dec << std::endl;

  return 0;
}