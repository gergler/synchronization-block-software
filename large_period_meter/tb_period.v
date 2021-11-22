module freDetect(clk, test, rst, d);  
  input clk;// опорная частота 
  input test; // сигнал для тестирования  
  input rst; // сброс сигнала  
    
  output d; // данные измерений 
 
  wire q;   // промежуточные данные  
  wire d;  

  // модуля управления  
  control u_control(.clk(clk), .rst(rst), .count_en(count_en), .latch_en(latch_en), .clear(clear));  
    
  // модуля счетчика  
  counter u_counter(.count_en(count_en), .clear(clear), .rst(rst), .test(test), .q(q)); 

  // модуля защелки  
  latch u_latch(.clk(clk), .rst(rst), .latch_en(latch_en), .q(q), .d(d));  
     
endmodule  
  
//Модуль управления  
// Модуль управления генерирует сигналы разрешения счета, сигналы разрешения защелки и сигналы сброса счетчика  
module control(clk, rst, count_en, latch_en, clear);  

  input clk;  
  input rst;  
  output count_en;  
  output latch_en;  
  output clear;  

  reg[1:0] state; // Сигнал состояния, используемый для управления различными сигналами включения  
  reg count_en;  
  reg latch_en;  
  reg clear;  

  always @(posedge clk or negedge rst)  
  if(!rst) // Сигнал сброса действителен  
    begin  // Удаляем различные разрешающие сигналы  
      state <= 2'd0;  
      count_en <= 1'b0;  
      latch_en <=1'b0;  
      clear <= 1'b0;  
    end  
   else // Когда следующий нарастающий фронт опорного сигнала встречаются, состояние меняется один раз, и состояние длится в течение 1 секунды после каждого изменения  
    begin  
      case(state)  
      	2'd0:   
          begin // Первый передний фронт приходит, начать подсчет, подсчитать число нарастающих фронтов сигнала при испытании в течение 1 опорного сигнала, это число является частота сигнала при испытании  
            count_en <= 1'b1;  // Сигнал включения подсчета действителен  
            latch_en <=1'b0;  
            clear <= 1'b0;  
            state <= 2'd1;  
          end  
        2'd1:  
          begin // Приходит второй нарастающий фронт, счет завершен, сигнал разрешения фиксации действителен, и измеренная частота фиксируется в защелке  
            count_en <= 1'b0;  
            latch_en <=1'b1;  
            clear <= 1'b0;  
            state <= 2'd2;  
          end  
        2'd2:   
          begin // Появляется третий нарастающий фронт, сигнал разрешения сброса действителен, и счетчик очищается для подготовки к следующему счету  
            count_en <= 1'b0;  
            latch_en <=1'b0;  
            clear <= 1'b1;  
            state <= 2'd0; // Состояние очищается и переходим к следующему измерению  
          end  
        default:  
          begin  
            count_en <= 1'b0;  
            latch_en <=1'b0;  
            clear <= 1'b0;  
            state <= 2'd0;  
          end          
      endcase             
    end      
endmodule  
  
// Модуль подсчета  
module counter(count_en, rst, clear, test, q);  
  input count_en;
  input rst;   // Сброс сигнала  
  input clear; // Очистить сигнал  
  input test;   // Сигнал для тестирования    
  output q; // Вывод счетчика
    
  reg q;  
    
  always @(posedge test or negedge rst) // Ввод нарастающего фронта измеряемого сигнала как чувствительного сигнала  
  if(!rst) // Сигнал сброса действителен, и выход счетчика очищается  
      begin  
        q <= 0;  
      end  
        
  else if(count_en) // Сигнал разрешения подсчета
      begin // q увеличивается на 1 каждый раз, когда достигается нарастающий фронт измеряемого сигнала  
          begin  
            q <= q + 1; 
          end  
      end  
        
   else if(clear) // Если сигнал очистки действителен, счетчик очищается, в основном используется для подготовки к следующему измерению  
      	begin  
          q <= 0;  
      	end  

   else  
   	begin  
    	  q <= q;  
    	end   
    
endmodule  
  
// Модуль защелки  
// Когда защелка включена, защелка будет защелкиваться и выводить выходное значение счетчика 
module latch(clk, latch_en, rst, q, d);  
              
  input clk, latch_en, rst;  
  input q;  
  output d;
  
  reg d;  
  reg[3:0] period;

  always @(posedge clk or negedge rst)  
  if(!rst) // Выход очищается, когда сигнал сброса действителен  
    begin  
      d <= 0; 
    end  
  else if(latch_en) // Когда сигнал защелки действителен, защелкиваем выходной сигнал счетчика на защелке  
    begin  
      d <= q; 
      if (d) 
	begin
	  period <= period + 1'b1;
	  $display("period/clk = %b", period);
	end
    end  
  else  // Когда не возникает ни одна из двух вышеперечисленных ситуаций, ввод остается неизменным  
    begin  
      d <= d; 
    end  
  
endmodule  
