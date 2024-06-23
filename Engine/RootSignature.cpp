#include "pch.h"
#include "RootSignature.h"

// RootSignature Ŭ������ Init �Լ� ����
void RootSignature::Init(ComPtr<ID3D12Device> device)
{
    CD3DX12_DESCRIPTOR_RANGE ranges[] =
    {
        CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0)
    };
    // ��Ʈ �Ķ���� �迭 ���� 
    CD3DX12_ROOT_PARAMETER param[1];
    param[0].InitAsDescriptorTable(_countof(ranges), ranges);

    // ��Ʈ ���� ���� ����ü �ʱ�ȭ (2���� ��Ʈ �Ķ���͸� ���)
    D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
    // ��Ʈ ������ �÷��� ���� (�Է� ������ �ܰ� ��� ����)
    sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    
    // ����ȭ�� ��Ʈ ������ ���� Blob ����
    ComPtr<ID3DBlob> blobSignature;
    // ���� �޽����� ���� Blob ����
    ComPtr<ID3DBlob> blobError;

    // ��Ʈ ������ ����ȭ (D3D12RootSignatureDesc -> ID3DBlob)
    ::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
    // ����ȭ�� ��Ʈ ������ ����Ͽ� ���� ��Ʈ ������ ����
    device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}
