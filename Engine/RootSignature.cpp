#include "pch.h"
#include "RootSignature.h"

// RootSignature 클래스의 Init 함수 정의
void RootSignature::Init(ComPtr<ID3D12Device> device)
{
    CD3DX12_DESCRIPTOR_RANGE ranges[] =
    {
        CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0)
    };
    // 루트 파라미터 배열 선언 
    CD3DX12_ROOT_PARAMETER param[1];
    param[0].InitAsDescriptorTable(_countof(ranges), ranges);

    // 루트 서명 설명 구조체 초기화 (2개의 루트 파라미터를 사용)
    D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
    // 루트 서명의 플래그 설정 (입력 조립기 단계 사용 가능)
    sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    
    // 직렬화된 루트 서명을 담을 Blob 선언
    ComPtr<ID3DBlob> blobSignature;
    // 에러 메시지를 담을 Blob 선언
    ComPtr<ID3DBlob> blobError;

    // 루트 서명을 직렬화 (D3D12RootSignatureDesc -> ID3DBlob)
    ::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
    // 직렬화된 루트 서명을 사용하여 실제 루트 서명을 생성
    device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}
