`timescale 1ns / 1ps
/* verilator lint_off WIDTH */
module RC4(
    input wire clk, // 时钟
    input wire rst, // 复位
    input wire [7:0] in, // 密钥或明文输入
    output reg [7:0] out // 密文输出
  );
  reg  mixing = 1, rec = 1;
  reg  [7:0] m [0:255]; // S 盒
  reg  [7:0] k [0:255]; // 记录输入的密钥
  reg  [7:0] x, y, i, j, n;
  wire [7:0] X, Y, a, b, t;
  integer _;
  assign t = j + m[i] + k[i % n];
  assign X = x + 1;
  assign a = m[X];
  assign Y = y + a;
  assign b = m[Y];
  always @(posedge clk) begin // 时钟上升沿进行密钥读取、密钥混淆、
    if (rst) begin // 当 rst 被置为 1 时，初始化所有寄存器并顺序读入密钥
      if (rec) begin // 在 rst 被置为 1 后的第一个时钟上升沿对各个寄存器进行初始化，rec 表示上一时钟周期 rst 的状态，若上一时钟周期 rst 为 1，则 rec 为 0，否则为 1
        for (_ = 0; _ < 256; _ = _ + 1) begin
          m[_] = _;
        end
        x <= 0;
        y <= 0;
        i <= 0;
        j <= 0;
        mixing <= 1;
        out <= 0;
        k[0] <= in; // 读入密钥第一位
        n <= 1;     // 并初始化密钥长度
      end else begin
        k[n] <= in; // 读入密钥的剩余位
        n <= n + 1; // 并记录密钥长度
      end
      rec <= 0;
    end else begin // 当 rst 被置为 0 时，进行密钥混淆和加密
      if (mixing) begin // mixing 值为 1 时进行密钥混淆
        m[i] <= m[t];
        m[t] <= m[i];
        i <= i + 1;
        j <= t;
        if (i == 255) begin
          mixing <= 0; // 将 mixing 置为 0，表示密钥混淆完成
        end
      end else begin // mixing 值为 0 时进行加（解）密
        m[X] <= b;
        m[Y] <= a;
        out <= m[a + b] ^ in;
        x <= X;
        y <= Y;
      end
      rec <= 1;
    end
  end
endmodule
