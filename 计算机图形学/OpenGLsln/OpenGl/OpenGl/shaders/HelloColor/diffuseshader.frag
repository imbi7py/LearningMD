#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ������
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;  //���㻷����
  	
    // ������ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);   //�������͹�Դ����нǿ϶�С��90�ȣ�Ҳ����˵���������㷶Χ��[0��1]
    vec3 diffuse = diff * lightColor;  //���㷴���
            
    vec3 result = (ambient + diffuse) * objectColor;  //�����Ϲ�
    FragColor = vec4(result, 1.0);
} 