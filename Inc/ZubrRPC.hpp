#include <ZubrIO.hpp>
#include <roboComBook.h>
#include <tuple>
#include <iostream>

namespace Zubr {
struct RPC {
  struct Messages {
#define GETTER(name, ind)                                                      \
  const auto &name() const { return std::get<ind>(Data); }                     \
  auto &name() { return std::get<ind>(Data); }

    // Single Field Wrapper
    template <typename T> struct SFW {
      std::tuple<T> Data;
      GETTER(Value, 0);
      SFW(T value) : Data{value} {}
      SFW() = default;
    };

    struct Empty {
      std::tuple<> Data;
    };

    using Int8 = SFW<int8_t>;
    using Int16 = SFW<int16_t>;
    using StateVec = std::array<int16_t, 25>;

    struct Param {
      std::tuple<int16_t, int32_t> Data;
      GETTER(Index, 0);
      GETTER(Value, 1);
    };

    struct UnitParam {
      std::tuple<int8_t, int8_t, int8_t, int16_t, int32_t> Data;
      GETTER(Sio, 0);
      GETTER(Oper, 1);
      GETTER(Id, 2);
      GETTER(Index, 3);
      GETTER(Value, 4);

      static constexpr int8_t Read = 1;
      static constexpr int8_t Write = 2;
    };

    struct State {
      std::tuple<int8_t, StateVec> Data;
      GETTER(Frame, 0);
      GETTER(Values, 1)
    };

    struct XYZ {
      int16_t X;
      int16_t Y;
      int16_t Z;
    };

    struct XYZW {
      int16_t X;
      int16_t Y;
      int16_t Z;
      int16_t W;
    };

    struct Imu {
      std::tuple<XYZ, XYZ, XYZW> Data;
      GETTER(Accel, 0);
      GETTER(Gyro, 1);
      GETTER(Quat, 2);
    };

    struct BaseTypes {
      template <typename T> static void Put(CsMessageOut &, const T &);
      template <typename T> static void Get(CsMessageIn &, T &);
    };

#undef GETTER

    template <typename Msg>
    static CsMessageOut ToCsMessageOut(const Msg &msg, char cmd) {
      CsMessageOut cs;
      cs.hostBeginQuery(cmd);

      std::apply([&cs](const auto & ... args) { (BaseTypes::Put(cs, args), ...); },
                 msg.Data);

      cs.hostEnd();

      return cs;
    }

    template <typename Msg> static Msg FromCsMessageIn(CsMessageIn &cs) {
      Msg msg;

      std::apply([&cs](auto & ... args) { (BaseTypes::Get<std::remove_reference_t<decltype(args)>>(cs, args), ...); },
                 msg.Data);

      return msg;
    }
  };

  struct Procedures {
    template <char CmdVal, char RspCmdVal, size_t RspSizeVal, typename RequestT,
              typename ResponceT>
    struct Procedure {
      static constexpr char Cmd = CmdVal;
      static constexpr char RspCmd = RspCmdVal;
      static constexpr size_t RspSize = RspSizeVal;
      using Request = RequestT;
      using Responce = ResponceT;
    };

#define PROCEDURE(name, cmd, rspCmd, rspSz, req, rsp)                                  \
  using name = Procedure<cmd, rspCmd, rspSz, Messages::req, Messages::rsp>;

    // Does not seem to work
    // Does not actually write to param
    // Needs testing
    PROCEDURE(SetZubrParam, RB_CB_PARAM_VALUE, RB_CB_PARAM_VALUE, 9, Param, Param);
    PROCEDURE(GetZubrParam, RB_CB_PARAM_VALUE_GET, RB_CB_PARAM_VALUE, 9, Int16, Param);

    PROCEDURE(UnitParam, RB_CB_UNIT_PARAM, RB_CB_UNIT_PARAM, 13, UnitParam, UnitParam);

    PROCEDURE(SetPosition, RB_CB_RL_CONTROL, RB_CB_RL_STATE, 61, State, State);
    PROCEDURE(GetPosition, RB_CB_RL_STATE_GET, RB_CB_RL_STATE, 61, Empty, State);
    PROCEDURE(GetVelocity, RB_CB_RL_VELO_GET, RB_CB_RL_STATE_VELO, 61, Empty, State);

    PROCEDURE(GetIMU, RB_CB_RL_IMU_GET, RB_CB_RL_IMU, 25, Empty, Imu);
    
    PROCEDURE(StartSlot, RB_CB_SLOT, RB_CB_SLOT_OK, 2, Int8, Empty);

#undef PROCEDURE
  };

  template <typename Proc>
  static typename Proc::Responce Call(IO &zubr,
                                      const typename Proc::Request &request = {}) {
    CsMessageOut csOut = Messages::ToCsMessageOut(request, Proc::Cmd);
    zubr.Send(csOut);
    CsMessageIn csIn = zubr.Recv(Proc::RspSize);

    if (csIn.hostCmd() != Proc::RspCmd)
      throw std::runtime_error(std::string("Tx/Rx Commands did not match: ") + Proc::Cmd + "->" + Proc::RspCmd);

    return Messages::FromCsMessageIn<typename Proc::Responce>(csIn);
  }
};

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int8_t &val) {
  msg.addInt8(val);
}
template <> void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int8_t &arg) {
  arg = msg.getInt8();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int16_t &val) {
  msg.addInt16(val);
}
template <> void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int16_t &arg) {
  arg = msg.getInt16();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const int32_t &val) {
  msg.addInt32(val);
}
template <> void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, int32_t &arg) {
  arg = msg.getInt32();
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const StateVec &values) {
  for (const auto &val : values)
    Put(msg, val);
}
template <>
void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, StateVec &arg) {
  for (int i = 0; i < arg.size(); ++i)
    Get(msg, arg[i]);
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const XYZ &val) {
  Put(msg, val.X);
  Put(msg, val.Y);
  Put(msg, val.Z);
}

template <> void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, XYZ &arg) {
  Get(msg, arg.X);
  Get(msg, arg.Y);
  Get(msg, arg.Z);
}

template <>
void RPC::Messages::BaseTypes::Put(CsMessageOut &msg, const XYZW &val) {
  Put(msg, val.X);
  Put(msg, val.Y);
  Put(msg, val.Z);
  Put(msg, val.W);
}

template <> void RPC::Messages::BaseTypes::Get(CsMessageIn &msg, XYZW &arg) {
  Get(msg, arg.X);
  Get(msg, arg.Y);
  Get(msg, arg.Z);
  Get(msg, arg.W);
}


}; // namespace Zubr