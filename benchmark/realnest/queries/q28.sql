SELECT 
  list_aggregate(tau_pt, 'variance'),
  list_aggregate(tau_eta, 'variance'),
  list_aggregate(jet_pt, 'variance'),
  list_aggregate(jet_eta, 'variance'),
  list_aggregate(muon_pt, 'variance'),
  list_aggregate(muon_eta, 'variance'),
  list_aggregate(el_pt, 'variance'),
  list_aggregate(el_eta, 'variance'),
  list_aggregate(ph_pt, 'variance'),
  list_aggregate(ph_eta, 'variance')  
FROM singleMu;