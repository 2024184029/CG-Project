#version 330 core
 layout(location = 0) in vec3 aPos;     // 정점 좌표 입력(바인딩 0)
 layout(location = 1) in vec3 aColor;   // 정점 색상 입력(바인딩 1)
 out vec3 vertexColor;                  // 프래그먼트 셰이더로 넘길 색상(interpolated)
 uniform mat4 model;                    // 모델 행렬(물체의 회전/이동/스케일)
 uniform mat4 view;                     // 뷰 행렬(카메라 위치/방향)
 uniform mat4 projection;               // 투영 행렬(원근 투영)

 void main()
 {
    gl_Position = projection * view * model * vec4(aPos, 1.0); // 표준 변환 순서: 

    vertexColor = aColor;                                      

}
